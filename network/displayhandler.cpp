#include "displayhandler.h"

DisplayHandler::DisplayHandler() : QThread() {

    moveToThread(this);
    _socket.moveToThread(this);
    connect(&_socket, SIGNAL(readyRead()), SLOT(read()));
    connect(&_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    qDebug("DisplayHandler (%p): constructed", this);
    this->start();

}

DisplayHandler::~DisplayHandler() {
    _socket.close();
    quit();
    wait();
    qDebug("DisplayHandler (%p): destroyed", this);
}

void DisplayHandler::pushJsonDoc(const QJsonDocument& jsonDoc) {
    qDebug() << "DisplayHandler: push new JSON doc";
    qDebug() << jsonDoc.toJson();
    QByteArray byteArray(jsonDoc.toJson(QJsonDocument::Compact));
    byteArray.append((char)4);   // EOT: End Of Transmission
    _socket.write(byteArray);
    _socket.flush();

}

void DisplayHandler::heatScoreCard() {
    qDebug("DisplayHandler (%p): thread started", this);
    try {
        exec();
    }
    catch (...) {
        qCritical("DisplayHandler (%p): an uncatched exception occured in the thread",this);
    }
    qDebug("DisplayHandler (%p): thread stopped", this);
}

void DisplayHandler::initConnection(int sd, const QJsonDocument &jsonDoc) {
    qDebug("DisplayHandler (%p): handle new connection", this);
    if (!_socket.setSocketDescriptor(sd)) {
        qCritical("DisplayHandler (%p): cannot initialize socket: %s", this,qPrintable(_socket.errorString()));
        return;
    }
    pushJsonDoc(jsonDoc);
}


void DisplayHandler::read() {

    // here read stuff
    QByteArray data = _socket.readLine(10000).trimmed();
    qDebug() << "DisplayHandler: Received from socket (NO USE). Read from socket: " << data;

}
