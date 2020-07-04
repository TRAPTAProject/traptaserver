#include "heatscorecard.h"
#include <QJsonArray>
#include <QDebug>

bool HeatScoreCard::x10;

HeatScoreCard::HeatScoreCard() {

}

HeatScoreCard::HeatScoreCard(const QList<Volley>& list) : QAbstractTableModel(),
    _volleyList(list)

{

}

HeatScoreCard::HeatScoreCard(const HeatScoreCard &copy) {
    _volleyList = copy.volleyList();
}

HeatScoreCard& HeatScoreCard::operator = (const HeatScoreCard& copy) {
    _volleyList = copy.volleyList();
    return *this;
}

bool HeatScoreCard::strikeAt(int volleyIndex) const {
    if (volleyIndex<0 || volleyIndex>=_volleyList.count()) return false;
    int score = _volleyList[volleyIndex].score();
    if (score==30 && !HeatScoreCard::x10) return true;
    if (score==60 && HeatScoreCard::x10) return true;
    return false;
}

int HeatScoreCard::rowCount(const QModelIndex& parent) const {
    return _volleyList.size();
}

int HeatScoreCard::columnCount(const QModelIndex &parent) const {
    return VOLLEY_MAX_ARROW+2;
}

QVariant HeatScoreCard::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role!=Qt::DisplayRole) return QVariant();
    if (orientation==Qt::Vertical) return QVariant(section+1);
    if (section>-1 && section<6) return QVariant(section+1);
    if (section==6) return QVariant("Volée");
    if (section==7) return QVariant("TOTAL");
    return QVariant();
}

QVariant HeatScoreCard::data(const QModelIndex& index, int role) const {
    if (role==Qt::TextAlignmentRole) return QVariant(Qt::AlignRight);
    if (role!=Qt::DisplayRole) return QVariant();
    int row = index.row();
    int col = index.column();
    if (row>=_volleyList.size()) return QVariant();
    if (col<VOLLEY_MAX_ARROW) {
        Volley volleyValue = volley(row);
        QList<int> arrowList = volleyValue.arrowList();
        if (col>=arrowList.size()) return QVariant();
        return QVariant(Volley::arrowLabelFromValue(arrowList.at(col)));
    }
    // volley score
    if (col==VOLLEY_MAX_ARROW) return QVariant(scoreAt(row));
    // total
    if (col==VOLLEY_MAX_ARROW+1) return QVariant(score(row));
    return QVariant();

}

int HeatScoreCard::criteria1() const {
    if (x10) return codeCount(11)+codeCount(10);
    return codeCount(10);
}

int HeatScoreCard::criteria2() const {
    if (x10) return codeCount(11);
    return codeCount(9);
}

QList<Volley> HeatScoreCard::volleyList() const {
    return _volleyList;
}

int HeatScoreCard::volleyCount() const {
    return _volleyList.size();
}

Volley HeatScoreCard::volley(int volleyIndex) const {
    if (volleyIndex<0 || volleyIndex>=_volleyList.size()) return Volley();
    return _volleyList.at(volleyIndex);
}

int HeatScoreCard::score() const {
    return score(-1);
}

// return cumulated score
int HeatScoreCard::score(int volleyIndex) const {
    int score = 0;
    int index = 0;
    foreach(Volley volley, _volleyList) {
        score+=volley.score();
        if (index==volleyIndex) break;
        if (volleyIndex>-1) index++;
    }
    return score;
}

QJsonArray HeatScoreCard::jsonArray() const {
    QJsonArray array;
    foreach (Volley volley, _volleyList) array.append(volley.jsonArray());
    return array;
}

QJsonArray HeatScoreCard::jsonArray(int volleyIndex) const {
    QJsonArray array;
    int volleyCount = volleyIndex+1;
    if (_volleyList.count()<volleyCount) volleyCount = _volleyList.count();
    for (int index=0; index<volleyCount; index++) array.append(_volleyList.at(index).jsonArray());
    return array;
}

int HeatScoreCard::scoreAt(int volleyIndex) const {
    if (volleyIndex<0 || volleyIndex>_volleyList.size()-1) return -1;
    return _volleyList[volleyIndex].score();
}

// return -1 if no volley in the list, otherwise return the number of arrows in the first volley
int HeatScoreCard::arrowCount() const {
    if (_volleyList.size()==0) return -1;
    return _volleyList.at(0).arrowList().size();
}

int HeatScoreCard::arrowDrawnCount() const {
    int counter = 0;
    foreach (Volley volley, _volleyList) counter += volley.arrowList().size();
    return counter;
}

int HeatScoreCard::codeCount(int code) const {
    int n = 0;
    foreach(Volley volley, _volleyList) {
        n += volley.codeCount(code);
    }
    return n;
}
