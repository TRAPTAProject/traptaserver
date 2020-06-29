#include "teamscore.h"

TeamScore::TeamScore(const QString& categ, const QString& name, const QList<Archer*>& archerList, int heatIndex, int volleyIndex, int score, QObject *parent) :
    QObject(parent),
    _name(name),
    _archerList(archerList),
    _categ(categ),
    _heatIndex(heatIndex),
    _volleyIndex(volleyIndex),
    _score(score)
{
}

TeamScore::TeamScore(const TeamScore &copy) {
    _name = copy.name();
    _categ = copy.categ();
    _archerList = copy.archerList();
    _heatIndex = copy.heatIndex();
    _volleyIndex = copy.volleyIndex();
    _score = copy.score();
}

TeamScore& TeamScore::operator = (const TeamScore& copy) {
    _name = copy.name();
    _categ = copy.categ();
    _archerList = copy.archerList();
    _heatIndex = copy.heatIndex();
    _volleyIndex = copy.volleyIndex();
    _score = copy.score();
    return *this;
}

QList<Archer*> TeamScore::archerList() const {
    return _archerList;
}

QString TeamScore::categ() const {
    return _categ;
}

int TeamScore::score() const {
    return _score;
}

QString TeamScore::name() const {
    return _name;
}

QStringList TeamScore::archerNameList() const {
    QStringList list;
    foreach (Archer* archer, _archerList) {
        list << QString("%0 : %1").arg(archer->name()).arg(archer->score(_heatIndex, _volleyIndex));
    }

    return list;
}

int TeamScore::heatIndex() const {
    return _heatIndex;
}

int TeamScore::volleyIndex() const {
    return _volleyIndex;
}
