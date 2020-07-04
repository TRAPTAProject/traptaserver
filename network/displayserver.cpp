#include "displayserver.h"

DisplayServer::DisplayServer(int port, DataModel* model) :
    QTcpServer() {

    _port = port;
    _dataModel = model;


}

void DisplayServer::processStartServer() {
    // Start listening
    if (_port==0) listen(QHostAddress::Any);
    else (listen(QHostAddress::Any, _port));
    qDebug() << "Trying to start listening on port " << _port;
    if (!isListening()) {
        qCritical() << "DisplayServer: Cannot start." << qPrintable(errorString());
        emit displayServerStarted(false, 0);
    }
    else {
        qDebug("DisplayServer: Listening on port %i",serverPort());
        emit displayServerStarted(true, serverPort());
    }
}

void DisplayServer::displayHandlerDisconnected() {
    qDebug() << "Display handler has been disconnected. DELETE IT !";
    delete QObject::sender();
}

void DisplayServer::incomingConnection(qintptr sd) {

    qDebug() << "Incoming connection ! Creating new displayHandler";

    DisplayHandler* displayHandler = new DisplayHandler();
    connect(_dataModel, SIGNAL(publishDisplay(QJsonDocument)), displayHandler, SLOT(pushJsonDoc(QJsonDocument)));
    connect(displayHandler, SIGNAL(disconnected()), this, SLOT(displayHandlerDisconnected()));


    QJsonDocument jsonDoc = _dataModel->getDisplayDocument();
    QMetaObject::invokeMethod(displayHandler, "initConnection", Qt::QueuedConnection,Q_ARG(int, sd), Q_ARG(QJsonDocument, jsonDoc));


}
