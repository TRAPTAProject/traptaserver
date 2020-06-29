#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <QThread>
#include "datamodel.h"
#include <QTcpSocket>
#include <QTimer>

class DisplayHandler : public QThread {
    Q_OBJECT
public:
    explicit DisplayHandler();
    virtual ~DisplayHandler();
    
signals:

    void disconnected();
    
protected:

    void heatScoreCard();

public slots:

    void initConnection(int sd, const QJsonDocument& jsonDoc);
    void pushJsonDoc(const QJsonDocument& jsonDoc);


private slots:

    void read();

private:

    QTcpSocket _socket;


    
};

#endif // DISPLAYHANDLER_H
