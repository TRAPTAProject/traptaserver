#ifndef TABLETHANDLER_H
#define TABLETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include "archers/volley.h"


class DataModel;

class TabletHandler : public QThread {
    Q_OBJECT
public:
    explicit TabletHandler(DataModel* dataModel);
    
    virtual ~TabletHandler();
    bool isBusy();
    void setBusy();

signals:

    void incomingHeatVolleyList(int id, int heatIndex, QList<Volley> volleyList);
    void incomingMatchVolleyList(int matchId, QList< QList<Volley> > list);
    void incomingMatchWinner(int matchId, int winner);
    void incomingMessage(quint32 ip, int targetId, int batteryLevel, int itemId, QString message);

private:

    DataModel* _dataModel;
    QTcpSocket _socket;
    QTimer _readTimer;
    bool _busy;
    QByteArray _byteArray;


    void reply(int code);
    void gotHello(int targetId);
    bool readByteArray(); // return true if end of transmission reached

protected:

    void heatScoreCard();

public slots:

    void handleConnection(int sd);

private slots:

    void readTimeout();
    void read();
    void disconnected();

};
    
#endif // TABLETHANDLER_H
