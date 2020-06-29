#ifndef UDPBROADCASTER_H
#define UDPBROADCASTER_H

#include <QThread>
#include <QTimer>
#include <QUdpSocket>

class UDPBroadcaster : public QThread {
    Q_OBJECT
public:
    explicit UDPBroadcaster(QObject *parent = 0);

    
signals:

    void broadcastError();
    void broadcasting();
    
public slots:
    
    void processConfigure(int command, int heatIndex, QString address, int portTablet, int portDisplay);
    void processTimeout();


private:

    QTimer _timer;
    QUdpSocket _socket;
    QString _address;
    int _portTablet;
    int _portDisplay;
    int _command;
    int _udpPort0;
    int _udpPort1;
    long _timestamp; // in seconds !
    int _heatIndex;
    QString _udpString;
    bool _doBroadcast;

};

#endif // UDPBROADCASTER_H
