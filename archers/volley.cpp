#include "volley.h"
#include <QJsonArray>
#include <QDebug>

Volley::Volley() {

    for (int i=0; i<VOLLEY_MAX_ARROW; i++) _arrowList << -1;
}

Volley::Volley(const QList<int>& list) {

    int counter = 0;
    foreach (int arrow, list) {
        _arrowList << arrow;
        counter++;
        if (counter==VOLLEY_MAX_ARROW) break;
    }
    for (int i=_arrowList.size(); i<VOLLEY_MAX_ARROW; i++) _arrowList << -1;

}

Volley::Volley(const QJsonArray& json) {

    int counter = 0;
    foreach (QJsonValue jsonArrow, json) {
        _arrowList << jsonArrow.toInt();
        counter++;
        if (counter==VOLLEY_MAX_ARROW) break;
    }
    for (int i=_arrowList.size(); i<VOLLEY_MAX_ARROW; i++) _arrowList << -1;
}

bool Volley::isVoid() const {
    foreach (int value, _arrowList)
        if (value>-1) return false;
    return true;
}

void Volley::setArrow(int index, int value) {
    if (index<0 || index>=_arrowList.size()) {
        qWarning() << "Index out of bound: " << index;
        return;
    }
    _arrowList[index] = value;
}

Volley::Volley(const Volley &copy) {
    _arrowList = copy.arrowList();
}

Volley& Volley::operator = (const Volley& copy) {
    _arrowList = copy.arrowList();
    return *this;
}

QJsonArray Volley::jsonArray() const {
    QJsonArray array;
    foreach (int arrow, _arrowList) array.append(arrow);
    return array;
}

int Volley::score() const {
    int score = 0;
    foreach(int value, _arrowList) {
        if (value>10) value=10;
        if (value<0) value = 0;
        score+=value;
    }
    return score;
}

QList<int> Volley::arrowList() const {
    return _arrowList;
}

bool Volley::missedOnce() const {
    foreach(int arrow, _arrowList) if (arrow==0) return true;
    return false;
}

int Volley::codeCount(int code) const {
    int n = 0;
    foreach(int arrow, _arrowList) if (arrow==code) n++;
    return n;
}


QString Volley::arrowLabelFromValue(int value) {
    if (value<0) return "";
    if (value==0) return "M";
    if (value>10) return "10X";
    return QString::number(value);
}
