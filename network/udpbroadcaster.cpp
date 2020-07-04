#include "udpbroadcaster.h"
#include <QDebug>
#include <QUdpSocket>
#include <QDateTime>
#include <QSettings>

#define BROADCAST_INTERVAL 3000

UDPBroadcaster::UDPBroadcaster(QObject *parent) :
    QThread(parent)
{
    QSettings settings;
    _udpPort0 = settings.value("udpport0", 5433).toInt();
    _udpPort1 = settings.value("udpport1", 5434).toInt();
    _address = "192.168.1.10";
    _portTablet = 8092;
    _portDisplay = 8093;
    moveToThread(this);
    _timer.moveToThread(this);
    _socket.moveToThread(this);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(processTimeout()));
    _doBroadcast = settings.value("broadcast", true).toBool();
    this->start();

}

void UDPBroadcaster::processConfigure(int command, int heatIndex, QString address, int portTablet, int portDisplay) {
    qDebug() << "Processing configure with command " << command << " address " << address << " port " << portTablet;
    _address = address;
    _portTablet = portTablet;
    _portDisplay = portDisplay;
    _command = command;
    _heatIndex = heatIndex;
    _timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    _udpString = QString::number(_timestamp);
    _udpString.append(',');
    _udpString.append(QString::number(_command));
    _udpString.append(',');
    _udpString.append(QString::number(_heatIndex));
    _udpString.append(',');
    _udpString.append(_address);
    _udpString.append(',');
    _udpString.append(QString::number(_portTablet));
    _udpString.append(',');
    _udpString.append(QString::number(_portDisplay));
    processTimeout();
    _timer.start(BROADCAST_INTERVAL);
}

void UDPBroadcaster::processTimeout() {

    if (!_doBroadcast) return;
    //qDebug() << "Broadcasting " <<  _udpPort0;
    emit broadcasting();
    qint64 byteCount = _socket.writeDatagram(_udpString.toLocal8Bit().constData(), QHostAddress::Broadcast, _udpPort0);
    if (byteCount<0) emit broadcastError();
    byteCount = _socket.writeDatagram(_udpString.toLocal8Bit().constData(), QHostAddress::Broadcast, _udpPort1);
    if (byteCount<0) emit broadcastError();


}
