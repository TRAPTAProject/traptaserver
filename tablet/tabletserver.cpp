#include "tabletserver.h"
#include <QTcpServer>


TabletServer::TabletServer(int port, DataModel* model) : QTcpServer() {
    _pool.setDataModel(model);
    _port = port;
}

TabletServer::~TabletServer() {
    close();

    qDebug("SocketListener: closed");

}


void TabletServer::processStartServer() {
    // Start listening
    qDebug() << "Trying to start listening on port " << _port;
    if (_port==0) listen(QHostAddress::Any);
    else (listen(QHostAddress::Any, _port));
    if (!isListening()) {
        qCritical() << "SocketListener: Cannot start." << qPrintable(errorString());
        emit tabletServerStarted(false, 0);
    }
    else {
        qDebug("SocketListener: Listening on port %i",serverPort());
        emit tabletServerStarted(true, serverPort());
    }
}

void TabletServer::incomingConnection(qintptr sd) {

    qDebug() << "Incoming connection !";
    TabletHandler* freeHandler = _pool.getConnectionHandler();

    // Let the handler process the new connection.
    if (freeHandler) {

        QMetaObject::invokeMethod(freeHandler, "handleConnection", Qt::QueuedConnection,Q_ARG(int, sd));

    }
    else {
        // Reject the connection
        qDebug("SocketListener: Too many incoming connections");

    }
}
