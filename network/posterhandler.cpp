#include "posterhandler.h"
#include <QJsonDocument>

PosterHandler::PosterHandler() :
    QThread()
{

    moveToThread(this);
    _socket.moveToThread(this);
    connect(&_socket, SIGNAL(readyRead()), SLOT(read()));
    connect(&_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    qDebug("PosterHandler (%p): constructed", this);
    this->start();
}

PosterHandler::~PosterHandler() {
    _socket.close();
    quit();
    wait();
    qDebug("PosterHandler (%p): destroyed", this);
}

void PosterHandler::pushJsonDoc(const QJsonDocument& jsonDoc) {
    qDebug() << "PosterHandler: push new JSON doc:";
    QByteArray byteArray(jsonDoc.toJson(QJsonDocument::Compact));
    byteArray.append((char)4);   // EOT: End Of Transmission
    _socket.write(byteArray);
    _socket.flush();

}

void PosterHandler::heatScoreCard() {
    qDebug("PosterHandler (%p): thread started", this);
    try {
        exec();
    }
    catch (...) {
        qCritical("PosterHandler (%p): an uncatched exception occured in the thread",this);
    }
    qDebug("PosterHandler (%p): thread stopped", this);
}

void PosterHandler::initConnection(int sd, const QJsonDocument &jsonDoc) {
    qDebug("PosterHandler (%p): handle new connection", this);
    if (!_socket.setSocketDescriptor(sd)) {
        qCritical("PosterHandler (%p): cannot initialize socket: %s", this,qPrintable(_socket.errorString()));
        return;
    }
    pushJsonDoc(jsonDoc);
}


void PosterHandler::read() {

    // here read stuff
    QByteArray data = _socket.readLine(10000).trimmed();
    qDebug() << "PosterHandler: Received from socket (NO USE). Read from socket: " << data;

}
