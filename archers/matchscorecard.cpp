#include "matchscorecard.h"
#include <QDebug>
#include <QJsonArray>


MatchScoreCard::MatchScoreCard() {

}

MatchScoreCard::MatchScoreCard(const QList<Volley>& list) : QAbstractTableModel(),
    _volleyList(list)

{

}

MatchScoreCard::MatchScoreCard(const MatchScoreCard &copy) {
    _volleyList = copy.volleyList();
}

MatchScoreCard& MatchScoreCard::operator = (const MatchScoreCard& copy) {
    _volleyList = copy.volleyList();
    return *this;
}

QList<Volley> MatchScoreCard::volleyList() const {
    return _volleyList;
}

int MatchScoreCard::rowCount(const QModelIndex& parent) const {
    return _volleyList.size();
}

int MatchScoreCard::columnCount(const QModelIndex &parent) const {
    return VOLLEY_MAX_ARROW+2;
}

// Warning: magic numbers !
QVariant MatchScoreCard::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role!=Qt::DisplayRole) return QVariant();
    if (orientation==Qt::Vertical) return QVariant(section+1);
    if (section>-1 && section<6) return QVariant(section+1);
    if (section==6) return QVariant("Volée");
    if (section==7) return QVariant("TOTAL");
    return QVariant();
}

QVariant MatchScoreCard::data(const QModelIndex& index, int role) const {
    if (role==Qt::TextAlignmentRole) return QVariant(Qt::AlignRight);
    if (role!=Qt::DisplayRole) return QVariant();
    int row = index.row();
    int col = index.column();
    if (row>=_volleyList.size()) return QVariant();
    if (col<VOLLEY_MAX_ARROW) {
        Volley v = volley(row);
        QList<int> arrowList = v.arrowList();
        if (col>=arrowList.size()) return QVariant();
        return QVariant(Volley::arrowLabelFromValue(arrowList.at(col)));
    }
    // volley score
    if (col==VOLLEY_MAX_ARROW) return QVariant(scoreAt(row));
    // total
    if (col==VOLLEY_MAX_ARROW+1) {
        //if (_scoreSet[row]>-1) return _scoreSet[row];  // if score set (means we are in set mode (not in point mode))
        return QVariant(score(row));
    }
    return QVariant();

}

Volley MatchScoreCard::volley(int volleyIndex) const {
    if (volleyIndex>=_volleyList.size()) return Volley();
    return _volleyList.at(volleyIndex);
}

int MatchScoreCard::score() const {
    return score(-1);
}

int MatchScoreCard::score(int volleyIndex) const {
    int score = 0;
    int index = 0;
    foreach(Volley volley, _volleyList) {
        score+=volley.score();
        if (index==volleyIndex) break;
        if (volleyIndex>-1) index++;
    }
    return score;
}

int MatchScoreCard::scoreAt(int volleyIndex) const {
    if (volleyIndex>_volleyList.size()-1 || volleyIndex<0) return -1;
    return _volleyList[volleyIndex].score();
}

int MatchScoreCard::volleyCount() const {
    return _volleyList.size();
}

// return -1 if no volley in the list, otherwise return the number of arrows in the first volley
int MatchScoreCard::arrowCount() const {
    if (_volleyList.size()==0) return -1;
    return _volleyList.at(0).arrowList().size();
}

int MatchScoreCard::arrowDrawn() const {
    int counter = 0;
    foreach (Volley volley, _volleyList) counter += volley.arrowList().size();
    return counter;
}

void MatchScoreCard::setScoreSet(int volleyIndex, int scoreSet) {
    if (volleyIndex>-1 || volleyIndex<MATCHSCORECARD_MAX_VOLLEY) _scoreSet[volleyIndex] = scoreSet;
}

QJsonArray MatchScoreCard::jsonArray() const {
    QJsonArray array;
    foreach(Volley volley, _volleyList) {
        array.append(volley.jsonArray());
    }
    return array;
}

int MatchScoreCard::codeCount(int code) const {
    int n = 0;
    foreach(Volley volley, _volleyList) {
        n += volley.codeCount(code);
    }
    return n;
}

