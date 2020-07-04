#ifndef POSTERSERVER_H
#define POSTERSERVER_H

#include <QTcpServer>
#include "database/datamodel.h"
#include "network/posterhandler.h"

class PosterServer : public QTcpServer {
    Q_OBJECT
public:
    explicit PosterServer(int port, DataModel* model);

signals:

    void posterServerStarted(bool started, int port);

protected:

    /** Serves new incoming connection requests */
    void incomingConnection(qintptr sd);


public slots:

    void processStartServer();
    void posterHandlerDisconnected();


private:

    int _port;
    DataModel* _dataModel;
    QList<PosterHandler*> _pool;


};

#endif // POSTERSERVER_H
