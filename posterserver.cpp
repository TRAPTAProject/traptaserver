#include "posterserver.h"

PosterServer::PosterServer(int port, DataModel* model) :
    QTcpServer() {

    _port = port;
    _dataModel = model;

}

void PosterServer::processStartServer() {
    // Start listening
    if (_port==0) listen(QHostAddress::Any);
    else (listen(QHostAddress::Any, _port));
    qDebug() << "Trying to start listening on port " << _port;
    if (!isListening()) {
        qCritical() << "DisplayServer: Cannot start." << qPrintable(errorString());
        emit posterServerStarted(false, 0);
    }
    else {
        qDebug("DisplayServer: Listening on port %i",serverPort());
        emit posterServerStarted(true, serverPort());
    }
}

void PosterServer::posterHandlerDisconnected() {
    qDebug() << "Poster handler has been disconnected. DELETE IT !";
    delete QObject::sender();
}

void PosterServer::incomingConnection(qintptr sd) {

    qDebug() << "Incoming connection ! Creating new displayHandler";

    PosterHandler* posterHandler = new PosterHandler();
    connect(_dataModel, SIGNAL(postDocument(QJsonDocument)), posterHandler, SLOT(pushJsonDoc(QJsonDocument)));
    connect(posterHandler, SIGNAL(disconnected()), this, SLOT(posterHandlerDisconnected()));

    QJsonDocument jsonDoc;
    //QJsonDocument jsonDoc = _dataModel->getInitPosterDocument();
    QMetaObject::invokeMethod(posterHandler, "initConnection", Qt::QueuedConnection,Q_ARG(int, sd), Q_ARG(QJsonDocument, jsonDoc));

}

