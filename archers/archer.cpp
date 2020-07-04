#include "archer.h"
#include <QJsonObject>
#include <QJsonArray>
#include "util.h"
#include "database/database.h"
#include <QDebug>
#include <QJsonDocument>


Archer::Archer( int id,
                const QString& license,
                const QString& category,
                const QString& firstname,
                const QString& lastname,
                const QString& noc,
                int shootId,
                int position,
                bool trispot,
                bool teammate,
                Database* db)

    : QObject(),

    _db(db),
    _license(license),
    _category(category),
    _firstname(firstname),
    _lastname(lastname),
    _noc(noc),
    _shootId(shootId),
    _position(position),
    _id(id),
    _drawId(1),
    _trispot(trispot),
    _teammate(teammate)

{

    _name = QString("%0 %1").arg(_lastname).arg(_firstname);
    for (int index=0; index<ARCHER_MAX_GLOBAL_VOLLEY; index++) _rank[index] = 0;

    for (int i=0; i<ARCHER_MAX_HEAT; i++) {
        _warning[i] = false;
        _scoreCardList.append(HeatScoreCard());
    }

}


HeatScoreCard Archer::scoreCard(int heatIndex) const {
    if (heatIndex<0) return _scoreCardList.first();
    if (heatIndex>=_scoreCardList.count()) return _scoreCardList.last();
    return _scoreCardList.at(heatIndex);
}

QList<HeatScoreCard> Archer::scoreCardList() const {
    return _scoreCardList;
}

// return true is same license and category
bool Archer::sameAs(Archer* archer) {
    if (archer==0) return false;
    if (_license==archer->license() && _category==archer->categ()) return true;
    return false;
}

void Archer::setDrawId(int draw) {
    _drawId = draw;
}

int Archer::drawId() const {
    return _drawId;
}


QString Archer::toString() const {

    return _id+" "+_license+" "+_category+" "+_name+" "+_noc;
}

QString Archer::toTitleString() const {
    return "Départ "+QString::number(_shootId)+"   "+Util::targetLabelFromPosition(_position)+"   "+_license+"   "+_category+"   "+_name;

}

QJsonObject Archer::jsonForPosition() const {
    QJsonObject obj;
    obj.insert("name", _name);
    obj.insert("club", _noc);
    obj.insert("position", Util::targetLabelFromPosition(_position));
    obj.insert("categ", _category);
    obj.insert("trispot", _trispot);
    return obj;
}


// returns: 2=mean of ten for the current volley, 1=we're good (equal or greater then before), 0:first volley, nothing to say. -1: not good, -2: got an M (missed)
int Archer::trend(int heatIndex, int volleyIndex) const {

    if (heatIndex==0 && volleyIndex==0) return 0;  // do not print if first volley
    int sum = 0;
    int counter = 0;
    // take first runs into account
    for (int i=0; i<heatIndex; i++) {
        HeatScoreCard heatScoreCard = _scoreCardList.at(i);
        for (int j=0; j<heatScoreCard.volleyCount(); j++) {
            int value = heatScoreCard.scoreAt(j);
            if (value>0) sum += value;
            counter++;
        }
    }
    // now for the heatIndex heatScoreCard
    HeatScoreCard heatScoreCard = _scoreCardList.at(heatIndex);
    if (heatScoreCard.volleyCount()-1<volleyIndex) return 0; // no trend because we havent reach that volley

    for (int i=0; i<volleyIndex; i++) {
        int value = heatScoreCard.scoreAt(i);
        if (value>0) sum += value;
        counter++;
    }

    float latest = (float)(heatScoreCard.scoreAt(volleyIndex));
    float mean = 0.0f;
    if (counter>0) mean = (float)sum/counter;
    int max = 30;
    if (HeatScoreCard::x10) max = 60;
    float mean2 = mean+(max-mean)/2;

    if (latest<(2*mean-max)) return -2;                  // -2: This is really bad
    if (latest>=mean2) return 2;                    // 2: excellent !
    if (latest>=mean) return 1;                     // 1: this is good !
    return -1;                                 // this is not good

}

float Archer::average() const {
    int count = arrowDrawnCount();
    if (count==0) return 0;
    int myScore = score();
    return ((float)myScore)/count;
}

int Archer::arrowDrawnCount() const {
    int count = 0;
    foreach (HeatScoreCard heatScoreCard, _scoreCardList) {
        count += heatScoreCard.arrowDrawnCount();
    }
    return count;
}

int Archer::volleyCount() const {
    int count = 0;
    foreach (HeatScoreCard heatScoreCard, _scoreCardList) {
        count += heatScoreCard.volleyList().size();
    }
    return count;
}

int Archer::volleyCount(int heatIndex) const {
    HeatScoreCard heatScoreCard = _scoreCardList.at(heatIndex);
    return heatScoreCard.volleyList().size();
}


QJsonObject Archer::jsonForRanking(int heatIndex, int volleyIndex, int globalVolleyIndex, bool includeTrend, bool includePosition) const {
    QJsonObject jsonObject;
    HeatScoreCard heatScoreCard = _scoreCardList.at(heatIndex);
    int count = heatScoreCard.volleyCount();
    if (count>volleyIndex+1) count = volleyIndex+1;
    int trendValue = 0;
    int volleyScore = heatScoreCard.scoreAt(volleyIndex);
    if (includeTrend && volleyScore>-1) trendValue = trend(heatIndex, volleyIndex);
    // determine rankingshift
    int rankingShift = 0;
    if (volleyScore>-1 && globalVolleyIndex>0 && _rank[globalVolleyIndex-1]>0 && _rank[globalVolleyIndex]>0) {
        rankingShift = _rank[globalVolleyIndex-1]-_rank[globalVolleyIndex];
    }
    // display happy if rankingshift is positive
    if (rankingShift>0 && trendValue<0) trendValue = 1;
    // display sad if rankingshift is negative
    if (rankingShift<0 && trendValue>0) trendValue = -1;
    jsonObject.insert("id", _id);
    jsonObject.insert("name", _name);
    jsonObject.insert("club", _noc);
    if (includePosition) jsonObject.insert("position", Util::targetLabelFromPosition(_position));

    jsonObject.insert("volleyScore", volleyScore);
    jsonObject.insert("trend", trendValue);
    jsonObject.insert("shift", rankingShift);
    jsonObject.insert("scorecard", jsonArrowList(heatIndex, volleyIndex));

    if (heatIndex>0) {
        jsonObject.insert("score1", heatScoreCard.score(volleyIndex));
        jsonObject.insert("total", score(heatIndex, volleyIndex));
        jsonObject.insert("score0", score(0));
    }
    else {
        jsonObject.insert("score0", heatScoreCard.score(volleyIndex));
    }
    return jsonObject;

}

void Archer::updateDB() {

    if (_db==0) return;
    bool ok = _db->updateArcher(
                _id,
                _license,
                _category,
                _firstname,
                _lastname,
                _noc,
                _position,
                _trispot,
                _teammate
                );
    if (!ok) {
        qCritical() << "Cannot update archer in DB";
    }
}

QJsonObject Archer::jsonForTablet() const {

    QJsonObject object;

    object.insert("id", _id);
    object.insert("license", _license);
    object.insert("category", _category);
    object.insert("name", _name);
    object.insert("shoot", _shootId);
    object.insert("letter", Util::letterFromPosition(_position));
    object.insert("trispot", _trispot);

    return object;

}

QJsonArray Archer::jsonArrowList() const {
    QJsonArray array;
    foreach (HeatScoreCard heatScoreCard, _scoreCardList) {
        array.append(heatScoreCard.jsonArray());
    }
    return array;
}

// array of array
QJsonArray Archer::jsonArrowList(int heatIndex, int volleyIndex) const {
    QJsonArray array;
    if (heatIndex<0) heatIndex = 0;
    if (heatIndex>=ARCHER_MAX_HEAT) heatIndex = 3;

    for (int i=0; i<=heatIndex; i++) {
        HeatScoreCard heatScoreCard = _scoreCardList.at(i);
        if (i<heatIndex) array.append(heatScoreCard.jsonArray()); // if not last heat, take whole scorecard
        else array.append(heatScoreCard.jsonArray(volleyIndex)); // if last, take up to volleyIndex
    }

    return array;
}


void Archer::resetScoreCard(int heatIndex) {
    if (heatIndex<0 || heatIndex>=_scoreCardList.count()) return;
    _scoreCardList[heatIndex] = HeatScoreCard();

}

void Archer::setScoreCard(int heatIndex, const HeatScoreCard &scoreCard, bool commit) {
    _scoreCardList[heatIndex] = scoreCard;
    if (commit) _db->updateHeatScoreCard(_id, heatIndex, scoreCard.volleyList());
}

// return the sum of all runs if index = -1
int Archer::score(int heatIndex) const {
    if (heatIndex<0 || heatIndex>=_scoreCardList.count()) return 0;
    return _scoreCardList.at(heatIndex).score();

}

int Archer::score(int heatIndex, int volleyIndex) const {
    if (heatIndex<0 || heatIndex>=_scoreCardList.count()) return 0;
    int total=0;
    // first the complete scorecards
    for (int i=0; i<heatIndex; i++) {
        total += _scoreCardList.at(i).score();
    }
    total += _scoreCardList.at(heatIndex).score(volleyIndex);
    return total;
}

int Archer::score() const {
    int total = 0;
    foreach(HeatScoreCard heatScoreCard, _scoreCardList) {
        total += heatScoreCard.score();
    }
    return total;
}

int Archer::criteria1() const {
    int n = 0;
    foreach(HeatScoreCard heatScoreCard, _scoreCardList) {
        n += heatScoreCard.criteria1();
    }
    return n;
}

int Archer::criteria2() const {
    int n = 0;
    foreach(HeatScoreCard heatScoreCard, _scoreCardList) {
        n += heatScoreCard.criteria2();
    }
    return n;
}

int Archer::codeCount(int value) const {

    int n = 0;
    foreach(HeatScoreCard heatScoreCard, _scoreCardList) {
        n += heatScoreCard.codeCount(value);
    }
    return n;
}

QString Archer::name() const {
    return _name;
}

QString Archer::firstName() const {
    return _firstname;
}

QString Archer::lastName() const {
    return _lastname;
}


QString Archer::license() const {
    return _license;
}

QString Archer::categ() const {
    return _category;
}

bool Archer::trispot() const {
    return _trispot;
}

bool Archer::teammate() const {
    return _teammate;
}

bool Archer::warned(int heatIndex) const {
    if (heatIndex<0 || heatIndex>=ARCHER_MAX_HEAT) return false;
    return _warning[heatIndex];
}

void Archer::setWarning(int heatIndex, bool value) {
    if (heatIndex<0 || heatIndex>=ARCHER_MAX_HEAT) return;
    _warning[heatIndex] = value;
}

QString Archer::noc() const {
    return _noc;
}

int Archer::id() const {
    return _id;
}

int Archer::shootId() const {
    return _shootId;
}

int Archer::position() const {
    return _position;
}

void Archer::setLicense(const QString& license) {
    _license = license;
   updateDB();

}

void Archer::setCategory(const QString& category) {
    _category = category;
    emit archerEdited(_id);
    updateDB();
}

void Archer::setName(const QString& name) {
    _name = name;
    QStringList list = name.split(' ');
    if (list.count()<2) {
        _firstname = "";
        _lastname = _name;
    }
    else {
        _firstname = list.last();
        list.removeLast();
        _lastname = list.join(' ');
    }
    emit archerEdited(_id);
    updateDB();
}

void Archer::setNoc(const QString& noc) {
    _noc = noc;
    emit archerEdited(_id);
    updateDB();
}

void Archer::setPosition(int position) {
    _position = position;
    emit archerEdited(_id);
    updateDB();
}

void Archer::setTrispot(bool value) {
    _trispot = value;
    emit archerEdited(_id);
    updateDB();
}

void Archer::setTeammate(bool value) {
    _teammate = value;
    updateDB();
}

void Archer::setShootId(int shootId) {
    _shootId = shootId;
    updateDB();
}

void Archer::setRank(int globalVolleyIndex, int rank) {
    if (globalVolleyIndex<0 || globalVolleyIndex>=ARCHER_MAX_GLOBAL_VOLLEY) return;
    _rank[globalVolleyIndex] = rank;

}
