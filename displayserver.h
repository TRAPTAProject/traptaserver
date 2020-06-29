#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <QTcpServer>
#include "datamodel.h"
#include <QMutex>
#include "displayhandler.h"

class DisplayServer : public QTcpServer {
    Q_OBJECT
public:
    explicit DisplayServer(int port, DataModel* model);
    
signals:
    
    void displayServerStarted(bool started, int port);

protected:

    /** Serves new incoming connection requests */
    void incomingConnection(qintptr sd);


public slots:

    void processStartServer();
    void displayHandlerDisconnected();
    

private:

    int _port;
    DataModel* _dataModel;
    QList<DisplayHandler*> _pool;


};

#endif // DISPLAYSERVER_H
