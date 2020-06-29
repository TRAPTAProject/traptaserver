#include "tabletinfo.h"
#include <QStringList>
#include <QTime>

TabletInfo::TabletInfo(int addr, QObject *parent) :
    QObject(parent) ,
    _addr(addr)

{

}

void TabletInfo::addMessage(const QString& msg) {
    QString date = QTime::currentTime().toString("hh:mm:ss.zzz").toLocal8Bit();
    _msg = date+" "+msg;
}

QString TabletInfo::toString() const {
    return QString("addr=%0 targ=%1 batt=%2% msg=%3").arg(_addr).arg(_targ).arg(_batt).arg(_msg);
}

int TabletInfo::addr() const {
    return _addr;
}

int TabletInfo::targ() const {
    return _targ;
}

int TabletInfo::batt() const {
    return _batt;
}

QString TabletInfo::msg() const {
    return _msg;
}

void TabletInfo::setBatt(int batt) {
    _batt = batt;
}

void TabletInfo::setTarg(int targ) {
    _targ = targ;
}
