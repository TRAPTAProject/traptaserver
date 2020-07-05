#include "match.h"
#include "matchscorecard.h"
#include <QDebug>
#include "archer.h"
#include <QJsonArray>
#include <QJsonObject>
#include "utils/util.h"
#include "database/database.h"

int Match::maxVolleyCount;

Match::Match(int id,
                int tabId,
                const QString &categ,
                int round,
                int rank,
                bool manual,
                int targetId,
                bool trispot,
                int tieBreakWinner,
                int mode, int player0, int player1,
                Database* db) :


    QObject(),
    _db(db),
    _categ(categ),
    _id(id),
    _manualRank(manual),
    _round(round),
    _rank(rank),
    _rankA(round),
    _rankB(rank),
    _tabId(tabId),
    _winner(-1),
    _targetId(targetId),
    _trispot(trispot),
    _tieBreakWinner(tieBreakWinner),
    _mode(mode)
{

    _archerId[0] = player0;
    _archerId[1] = player1;
    for (int i=0; i<2; i++) {
        _archerName[i] = QString("");
        _score[i]  =0;
        _latestVolley[i] = 0;
        for (int j=0; j<5; j++) {
            _scoreSet[i][j] = -1;
        }
    }

}

void Match::setDB(Database* db) {
    _db = db;
}

void Match::setId(int id) {
    _id = id;
}

bool Match::manualRank() {
    return _manualRank;
}

MatchScoreCard Match::scoreCard(int archerIndex) {
    if (archerIndex<0 || archerIndex>1) return MatchScoreCard();
    return _scorecard[archerIndex];
}

// sent to tablets
QJsonObject Match::jsonDataSet() const {
    QJsonObject object;

    object.insert("matchId", _id);
    object.insert("trispot", _trispot);
    object.insert("matchmode", _mode);
    QJsonArray jsonArray;
    for (int i=0; i<2; i++) {
        QJsonObject jsonArcher;
        QString name = "Bye";
        if (!_archerName[i].isEmpty()) name = _archerName[i];
        jsonArcher.insert("name", name);
        jsonArcher.insert("trispot", _trispot);
        jsonArray.append(jsonArcher);
    }
    object.insert("archerList", jsonArray);
    return object;
}



QJsonObject Match::jsonResultSet() const {
    QJsonObject object;
    object.insert("tieBreakWinner", _tieBreakWinner);
    QJsonArray jsonArray;
    jsonArray.append(_scorecard[0].jsonArray());
    jsonArray.append(_scorecard[1].jsonArray());
    object.insert("arrowList", jsonArray);
    return object;
}

QJsonArray Match::markerList() const {
    QJsonArray json;
    if (_mode>0) return json;
    int min = _scorecard[0].volleyCount();
    if (_scorecard[1].volleyCount()<min) min = _scorecard[1].volleyCount();
    for (int i=0; i<min; i++) {
        if (_scorecard[0].scoreAt(i)>_scorecard[1].scoreAt(i)) json.append(0);
        else if (_scorecard[0].scoreAt(i)<_scorecard[1].scoreAt(i)) json.append(1);
        else json.append(-1);
    }
    return json;
}

QJsonObject Match::jsonForDisplay() const {
    QJsonObject obj;
    obj.insert("target", _targetId);
    obj.insert("categ", _categ);
    obj.insert("markerList", markerList());
    int value = _winner;
    if (_tieBreakWinner>-1) value = _tieBreakWinner;
    obj.insert("winner", value);
    obj.insert("tieBreak", _tieBreakWinner>-1);
    QJsonArray jsonArray;

    for (int i=0; i<2; i++) {
        QJsonObject archer;
        if (_manualRank && i==0) archer.insert("initialRank", _rankA);
        else if (_manualRank && i==1) archer.insert("initialRank", _rankB);
        else archer.insert("initialRank", Util::getInitRank(i, _round, _rank));
        QString name = "Bye";
        if (!_archerName[i].isEmpty()) name = _archerName[i];
        archer.insert("name", name);
        archer.insert("score", _score[i]);
        QJsonArray jsonVolleyList;
        for (int index=0; index<_scorecard[i].volleyCount(); index++) {
            jsonVolleyList.append(_scorecard[i].scoreAt(index));
        }
        archer.insert("volleyList", jsonVolleyList);
        jsonArray.append(archer);
    }

    obj.insert("archerList", jsonArray);
    return obj;
}

void Match::updateDB() {
    if (_db==0) return;
    bool ok = _db->updateMatch(_id,
                               _targetId,
                               _trispot,
                               _tieBreakWinner,
                               _mode,
                               _archerId[0],
                               _archerId[1]);
    if (!ok) {
        qCritical() << "Cannot update match " << _id;
    }

}

int Match::round() {
    return _round;
}

int Match::rank() {
    return _rank;
}

int Match::targetId() {
    return _targetId;
}

void Match::setMode(int mode) {
    _mode = mode;
    updateDB();
}

int Match::mode() {
    return _mode;
}

int Match::score(int index) {
    if (index<0 || index>1) return -1;
    return _score[index];
}

void Match::setScoreCard(const MatchScoreCard& scoreCard0, const MatchScoreCard& scoreCard1, bool commit) {

    _scorecard[0] = scoreCard0;
    _scorecard[1] = scoreCard1;

    _score[0] = 0;
    _score[1] = 0;

    if (_mode==modeSet) {
        for (int volleyIndex=0; volleyIndex<MATCHSCORECARD_MAX_VOLLEY; volleyIndex++) {
            int score0 = _scorecard[0].scoreAt(volleyIndex);
            int score1 = _scorecard[1].scoreAt(volleyIndex);
            if (score0<0 || score1<0) {
                _scoreSet[0][volleyIndex] = -1;
                _scoreSet[1][volleyIndex] = -1;
                _scorecard[0].setScoreSet(volleyIndex, -1);
                _scorecard[1].setScoreSet(volleyIndex, -1);
            }
            else if (score0==score1) {
                _scoreSet[0][volleyIndex] = 1;
                _scoreSet[1][volleyIndex] = 1;
                _scorecard[0].setScoreSet(volleyIndex, 1);
                _scorecard[1].setScoreSet(volleyIndex, 1);
            }
            else if (score0>score1) {
                _scoreSet[0][volleyIndex] = 2;
                _scoreSet[1][volleyIndex] = 0;
                _scorecard[0].setScoreSet(volleyIndex, 2);
                _scorecard[1].setScoreSet(volleyIndex, 0);
            }
            else if (score0<score1) {
                _scoreSet[0][volleyIndex] = 0;
                _scoreSet[1][volleyIndex] = 2;
                _scorecard[0].setScoreSet(volleyIndex, 0);
                _scorecard[1].setScoreSet(volleyIndex, 2);
            }
            if (_scoreSet[0][volleyIndex]>0) _score[0]+=_scoreSet[0][volleyIndex];
            if (_scoreSet[1][volleyIndex]>0) _score[1]+=_scoreSet[1][volleyIndex];

        }


        int winnerMaxVolley = maxVolleyCount;

        // check if there is a winner
        if (_score[0]>winnerMaxVolley) {
            _winner = 0;
            _tieBreakWinner = -1;
        }
        else if (_score[1]>winnerMaxVolley) {
            _winner = 1;
            _tieBreakWinner = -1;
        }
        else _winner = -1;

    }
    else {
        _score[0] = _scorecard[0].score();
        _score[1] = _scorecard[1].score();
        if (_scorecard[0].volleyCount()==Match::maxVolleyCount &&
            _scorecard[1].volleyCount()==Match::maxVolleyCount) {
            if (_score[0]>_score[1]) {
                _winner = 0;
                _tieBreakWinner = -1;
            }
            else if (_score[0]<_score[1]) {
                _winner = 1;
                _tieBreakWinner = -1;
            }
            else _winner = -1;

        }

    }

    if (commit) {
        QList< QList<Volley> > list;
        list << scoreCard0.volleyList();
        list << scoreCard1.volleyList();
        _db->updateMatchScoreCard(_id, list);
    }

}

void Match::setTargetId(int targetId) {
    _targetId = targetId;
    updateDB();


}

// comes from ui (combo box index changed)
void Match::setArcherId(int index, int archerId) {
    if (index<0 || index>1) return;
    _archerId[index] = archerId;
    updateDB();


}

void Match::setArcherName(int index, const QString& name) {
    if (index<0 || index>1) return;
    _archerName[index] = name;

}


QString Match::archerName(int index) const {
    if (index<0 || index>1) return QString();
    return _archerName[index];
}

int Match::archerId(int index) {
    if (index<0 || index>1) return 0;
    return _archerId[index];
}

void Match::setWinner(int index) {
    _tieBreakWinner = index;
    updateDB();

}

int Match::winner() {
    if (_winner>-1) return _winner;
    return _tieBreakWinner;
}

int Match::looser() {
    return 1-winner();
}

QString Match::categ() const {
    return _categ;
}

int Match::id() {
    return _id;
}

int Match::tabId() {
    return _tabId;
}

int Match::tieBreak() {
    return _tieBreakWinner;
}

bool Match::trispot() {
    return _trispot;
}

void Match::setTrispot(bool value) {
    _trispot = value;
    updateDB();
}

Match::~Match() {
    qDebug() << "Destroying Match " << _id;
}
