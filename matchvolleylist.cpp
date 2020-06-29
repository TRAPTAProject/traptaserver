#include "matchvolleylist.h"
#include <QSqlQuery>
#include <QDebug>
#include <QJsonArray>

MatchVolleylist::MatchVolleylist(int matchId, int archerIndex, QObject *parent) :
    QAbstractTableModel(parent)
{

    _matchId = matchId;
    _archerIndex = archerIndex;
    _db = 0;
    for (int i=0; i<MATCHVOLLEYLIST_MAX_VOLLEY; i++) _scoreSet[i] = -1;

}

int MatchVolleylist::rowCount(const QModelIndex& parent) const {
    return _volleyList.size();
}

int MatchVolleylist::columnCount(const QModelIndex &parent) const {
    return VOLLEY_MAX_ARROW+2;
}

// Warning: magic numbers !
QVariant MatchVolleylist::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role!=Qt::DisplayRole) return QVariant();
    if (orientation==Qt::Vertical) return QVariant(section+1);
    if (section>-1 && section<6) return QVariant(section+1);
    if (section==6) return QVariant("Volée");
    if (section==7) return QVariant("TOTAL");
    return QVariant();
}

QVariant MatchVolleylist::data(const QModelIndex& index, int role) const {
    if (role==Qt::TextAlignmentRole) return QVariant(Qt::AlignRight);
    if (role!=Qt::DisplayRole) return QVariant();
    int row = index.row();
    int col = index.column();
    if (row>=_volleyList.size()) return QVariant();
    if (col<VOLLEY_MAX_ARROW) {
        Volley volley = getVolley(row);
        QList<int> arrowList = volley.arrowList();
        if (col>=arrowList.size()) return QVariant();
        return QVariant(Volley::arrowLabelFromValue(arrowList.at(col)));
    }
    // volley score
    if (col==VOLLEY_MAX_ARROW) return QVariant(getScoreAt(row));
    // total
    if (col==VOLLEY_MAX_ARROW+1) {
        if (_scoreSet[row]>-1) return _scoreSet[row];  // if score set (means we are in set mode (not in point mode))
        return QVariant(getScore(row));
    }
    return QVariant();

}

void MatchVolleylist::setDatabase(QSqlDatabase* db) {
    _db = db;
}

Volley MatchVolleylist::getVolley(int volleyIndex) const {
    if (volleyIndex>=_volleyList.size()) return Volley();
    return _volleyList.at(volleyIndex);
}

int MatchVolleylist::getScore() const {
    return getScore(-1);
}

int MatchVolleylist::getScore(int volleyIndex) const {
    int score = 0;
    int index = 0;
    foreach(Volley volley, _volleyList) {
        score+=volley.score();
        if (index==volleyIndex) break;
        if (volleyIndex>-1) index++;
    }
    return score;
}

int MatchVolleylist::getScoreAt(int volleyIndex) const {
    if (volleyIndex>_volleyList.size()-1 || volleyIndex<0) return -1;
    return _volleyList[volleyIndex].score();
}

int MatchVolleylist::getVolleyCount() const {
    return _volleyList.size();
}

// return -1 if no volley in the list, otherwise return the number of arrows in the first volley
int MatchVolleylist::getArrowCount() const {
    if (_volleyList.size()==0) return -1;
    return _volleyList.at(0).arrowList().size();
}

int MatchVolleylist::getArrowDrawn() const {
    int counter = 0;
    foreach (Volley volley, _volleyList) counter += volley.arrowList().size();
    return counter;
}

void MatchVolleylist::setScoreSet(int volleyIndex, int scoreSet) {
    if (volleyIndex>-1 || volleyIndex<MATCHVOLLEYLIST_MAX_VOLLEY) _scoreSet[volleyIndex] = scoreSet;
}

int MatchVolleylist::getScoreSet(int volleyIndex) const {
    if (volleyIndex>-1 || volleyIndex<MATCHVOLLEYLIST_MAX_VOLLEY) return _scoreSet[volleyIndex];
    return -1;
}


// CREATE TABLE `matchtable` (`matchId` INTEGER PRIMARY KEY AUTOINCREMENT, `tabid` INTEGER, `categ` char(10), `target` INTEGER, `trispot` INTEGER, `round` INTEGER, `rank` INTEGER, `archer0` INTEGER, `archer1` INTEGER, `winner` INTEGER, `mode` INTEGER);
// CREATE TABLE `settable` (`index` INTEGER PRIMARY KEY AUTOINCREMENT, `match` INTEGER, `archer` INTEGER, `volley` INTEGER, `arrow0` INTEGER, `arrow1` INTEGER, `arrow2` INTEGER, `arrow3` INTEGER, `arrow4` INTEGER, `arrow5` INTEGER);

void MatchVolleylist::setVolleyList(const QList<Volley>& list) {

    _volleyList.clear();
    foreach (Volley volley, list)
        if (!volley.isVoid()) _volleyList << volley;

    if (_db!=0) {
        QSqlQuery query(*_db);
        QString str = QString("UPDATE `settable` SET `arrow0`= :arrow0, `arrow1`= :arrow1, `arrow2`= :arrow2, `arrow3`= :arrow3, `arrow4`= :arrow4, `arrow5`= :arrow5  WHERE (`match`=='%0' AND `archer`='%1' AND `volley`= :volley)")
                .arg(_matchId).arg(_archerIndex);
        qDebug() << str;
        query.prepare(str);
        _db->transaction();
        for (int volleyIndex=0; volleyIndex<MATCHVOLLEYLIST_MAX_VOLLEY; volleyIndex++) {
            Volley volley = getVolley(volleyIndex);
            QList<int> arrowList = volley.arrowList();
            query.bindValue(":arrow0", arrowList.at(0));
            query.bindValue(":arrow1", arrowList.at(1));
            query.bindValue(":arrow2", arrowList.at(2));
            query.bindValue(":arrow3", arrowList.at(3));
            query.bindValue(":arrow4", arrowList.at(4));
            query.bindValue(":arrow5", arrowList.at(5));
            query.bindValue(":volley", volleyIndex);
            query.exec();
        }
        _db->commit();
    }

}

QJsonArray MatchVolleylist::jsonArray() const {
    QJsonArray array;
    foreach(Volley volley, _volleyList) {
        array.append(volley.jsonArray());
    }
    return array;
}

int MatchVolleylist::getCodeCount(int code) const {
    int n = 0;
    foreach(Volley volley, _volleyList) {
        n += volley.codeCount(code);
    }
    return n;
}

