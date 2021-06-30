#include "tablethandler.h"
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include <QJsonValue>
#include <QHostAddress>

#define READ_TIMEOUT 6000
#define COMMAND_NEEDCONF 0
#define COMMAND_VOLLEY 1
#define KEY_COMMAND "command"

#define EOT 4

TabletHandler::TabletHandler(DataModel *dataModel) : QThread() {

    _dataModel = dataModel;
    _busy = false;
    moveToThread(this);
    _socket.moveToThread(this);
    _readTimer.moveToThread(this);
    connect(this, SIGNAL(incomingHeatVolleyList(int,int,QList<Volley>)), _dataModel, SLOT(processIncomingHeatVolleyList(int,int,QList<Volley>)));
    connect(this, SIGNAL(incomingMessage(quint32,int,int,int,QString)), _dataModel, SLOT(processIncomingMessage(quint32,int,int,int,QString)));
    connect(this, SIGNAL(incomingMatchVolleyList(int, QList< QList<Volley> >)), _dataModel, SLOT(processIncomingMatchVolleyList(int, QList< QList<Volley> >)));
    connect(this, SIGNAL(incomingMatchWinner(int,int)), _dataModel, SLOT(processIncomingMatchWinner(int,int)));
    connect(&_socket, SIGNAL(readyRead()), SLOT(read()));
    connect(&_socket, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(&_readTimer, SIGNAL(timeout()), SLOT(readTimeout()));
    _readTimer.setSingleShot(true);
    qDebug("tabletHandler (%p): constructed", this);
    this->start();

}

TabletHandler::~TabletHandler() {
    _socket.close();
    quit();
    wait();
    qDebug("tabletHandler (%p): destroyed", this);
}


void TabletHandler::heatScoreCard() {
    qDebug("tabletHandler (%p): thread started", this);
    try {
        exec();
    }
    catch (...) {
        qCritical("tabletHandler (%p): an uncatched exception occured in the thread",this);
    }
    qDebug("tabletHandler (%p): thread stopped", this);
}


void TabletHandler::handleConnection(int sd) {
    qDebug("tabletHandler (%p): handle new connection", this);
    _busy = true;
    //UGLY workaround - we need to clear writebuffer before reusing this socket
    //https://bugreports.qt-project.org/browse/QTBUG-28914
    _socket.connectToHost("",0);
    _socket.abort();
    if (!_socket.setSocketDescriptor(sd)) {
        qCritical("tabletHandler (%p): cannot initialize socket: %s", this,qPrintable(_socket.errorString()));
        return;
    }

    _readTimer.start(READ_TIMEOUT);
    _byteArray.clear();

}


bool TabletHandler::isBusy() {
    return _busy;
}

void TabletHandler::setBusy() {
    this->_busy = true;
}


void TabletHandler::readTimeout() {
    qDebug("tabletHandler (%p): read timeout occured",this);
    _socket.disconnectFromHost();
}


void TabletHandler::disconnected() {
    qDebug("tabletHandler (%p): disconnected", this);
    _socket.close();
    _readTimer.stop();
    _busy = false;
}

void TabletHandler::reply(int code) {
    QJsonObject jsonObject;
    jsonObject.insert("response", code);
    QJsonDocument doc(jsonObject);
    //qDebug() << "sending back: " << doc.toJson();
    QByteArray byteArray(doc.toJson());
    byteArray.append('\n');
    byteArray.append("EOT\n");   // End Of Transmission
    _socket.write(byteArray);
    _socket.flush();
    _readTimer.start(READ_TIMEOUT);
}

void TabletHandler::gotHello(int targetId) {

    int ip = _socket.peerAddress().toIPv4Address();
    qDebug() << "Got HELLO from " << ip << " dealing with target " << targetId;
    emit incomingMessage(ip, targetId, -1, -1, "dit bonjour");
    _socket.disconnectFromHost();
}

/*
    input:
    {
        "command":0 (send heatScoreCard),
        "targetId":2,
        "data": {
            "archerId":123,
            "heatIndex":0,
            "arrowList": [[2,3,4], [5,6,7], [8,9,8]]
        }
    }
    {
        "command":1 (archer list),


    }
    {
        "command":2 (arrows for archers) (init qualif)
        "targetId":3,
        "data": [archerId1, archerId2...]

    }
    {
        "command":3 (hello)
        "targetId":4

    }

    {
        "command":5 (fetch result for match)
        "targetId":2
        "matchId":3

    }


    {
        "command":6 (send match)
        "targetId":2
        "data": {
            "matchId":3
            "arrowList": [[[2,3,4], [5,6,7]],[[1,2,3],[4,5,6]]
            "winner":-1 (undefined), 0 or 1
        }

    }


    output:
    {
        "response":0


    }
    { (command 1)
        "response":0,
        "volleyCount":10,
        "arrowCount":3,
        "x10":false,
        "data": {
                "1": [
                {
            "category": "CHCL",
            "club": "GHOSTS",
            "id": 1,
            "license": "123001T",
            "name": "NEARLY-HEADLESS NICK",
            "position": 0,
            "start": 1
        },
    }
    {
        "response":0,
        "data": {
            "archerId": [  [[1,2,3], [4,5,6]], [] ],
            "archerId1": [  [], [] ],


        }

    }

    command 1: (list of matches)
    {
        "response":0,


        "data": {


            "1": {
                "matchId": xx,
                "trispot": 1,
                "categ": "CHCL",
                "archerList" : [ {"name":"name", "licence":FGHDJHFHD}, {archer1 name, licence}],

            }
        }

    }

    command 5:
    {
        "response":0,
        // table of volley
        "data": {
                "winner":-1,
                arrowList:  [  [[1,2,3], [4,5,6]], [] ],
                            [  [], [] ],


        }

    }


*/

bool TabletHandler::readByteArray() {
    char c;
    bool eot = false;
    while (!eot) {
        bool ok = _socket.getChar(&c);
        if (!ok) break;
        if (c==(char)EOT) {
            eot = true;
            break;
        }
        if (c!='\n') _byteArray.append(c);
    }
    _readTimer.start(READ_TIMEOUT);
    return eot;
}

void TabletHandler::read() {

    // here read stuff
    bool eot = readByteArray();
    qDebug() << "Read from socket:" << QString(_byteArray);
    if (!eot) return;

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(_byteArray, &parseError);
    _byteArray.clear(); // clear byte array, ready for another transmission
    if (jsonDoc.isNull()) {
        qDebug() << "JSON Doc is null";
        return;
    }
    if (parseError.error==QJsonParseError::NoError) {
        QJsonObject rootObject = jsonDoc.object();
        if (rootObject.isEmpty()) {
            qDebug() << "Root object is empty";
            return reply(-1);
        }
        int command = rootObject.value(KEY_COMMAND).toInt();
        QJsonObject response;

        switch (command) {

            case 1: {        // ask for list of archers or list of matches
                int workingMode = _dataModel->getWorkingMode();
                QJsonObject dataObject;
                if (workingMode==0) {  // if qualif (individual)
                    dataObject = _dataModel->jsonForTargetList();
                    response.insert("response", 0);
                    response.insert("heatVolleyCount", _dataModel->heatVolleyCount()); // # of volley
                    response.insert("arrowCount", _dataModel->heatArrowCount()); // # of arrow
                    response.insert("x10", _dataModel->heat10X());
                    response.insert("roundCount", _dataModel->roundCount());
                }
                else if (workingMode==1) {  // if matches
                    dataObject = _dataModel->jsonForMatchList();
                    response.insert("response", 1);
                    response.insert("matchVolleyMax", _dataModel->matchVolleyMax());
                    response.insert("arrowCount", _dataModel->matchArrowCount());
                    response.insert("x10", false);

                }
                if (dataObject.isEmpty()) {
                    if (workingMode==0) return reply(-1);
                    else if (workingMode==1) return reply(-2);
                    else return reply(-3);
                }
                response.insert("data", dataObject);
                break;
            }
            case 2: {   // ask for results (qualif)
                int targetId = rootObject.value("targetId").toInt();
                int batteryLevel = rootObject.value("batteryLevel").toInt();
                QJsonArray archerList = rootObject.value("data").toArray();
                QList<int> archerIdList;
                int archerCount = archerList.size();
                for (int index=0; index<archerCount; index++) {
                    double value = archerList.at(index).toDouble();
                    archerIdList.append((int)value);
                }
                QJsonObject dataObject = _dataModel->jsonForArrowList(archerIdList);
                if (dataObject.isEmpty()) return reply(-1);
                response.insert("data", dataObject);
                response.insert("response", 0);
                emit incomingMessage((quint32)_socket.peerAddress().toIPv4Address(), targetId, batteryLevel, 0, "Feuilles récupérées");
                break;
            }
            case 0: {   // receiving heatScoreCard for qualif
                qDebug() << "Receiving heatScoreCard for qualif";
                int targetId = rootObject.value("targetId").toInt();
                int batteryLevel = rootObject.value("batteryLevel").toInt();
                QJsonObject info = rootObject.value("data").toObject();
                int archerId = info.value("archerId").toInt();
                int heatIndex = info.value("heatIndex").toInt();
                QJsonArray jsonArrowList = info.value("arrowList").toArray();
                QList<Volley> volleyList;
                foreach (QJsonValue jsonVolley, jsonArrowList) volleyList << Volley(jsonVolley.toArray());
                emit incomingHeatVolleyList(archerId, heatIndex, volleyList);
                response.insert("response", 0);

                emit incomingMessage((quint32)_socket.peerAddress().toIPv4Address(), targetId, batteryLevel, archerId, "Volée reçue");
                break;
            }
            case 3: {
                // hello broadcast
                int targetId = rootObject.value("targetId").toInt();
                return gotHello(targetId);
                break;
            }
            case 5: // ask for results (matches)
                {
                int targetId = rootObject.value("targetId").toInt();
                int matchId = rootObject.value("matchId").toInt();
                int batteryLevel = rootObject.value("batteryLevel").toInt();
                Match* match = _dataModel->getMatchMap().value(matchId);
                if (match==0) return reply(-1);
                response.insert("data", match->jsonResultSet());
                response.insert("response", 0);

                emit incomingMessage((quint32)_socket.peerAddress().toIPv4Address(), targetId, batteryLevel, matchId, "Match renvoyé");
                break;
            }
            case 6: {
                // receiving matches
                qDebug() << "Receiving match";
                int targetId = rootObject.value("targetId").toInt();
                int batteryLevel = rootObject.value("batteryLevel").toInt();
                QJsonObject info = rootObject.value("data").toObject();
                int matchId = info.value("matchId").toInt();
                int winner = info.value("tieBreakWinner").toInt();
                emit incomingMatchWinner(matchId, winner);

                QJsonArray jsonArrowList = info.value("arrowList").toArray();
                QList< QList<Volley> > arrowList;
                for (int archerIndex=0; archerIndex<2; archerIndex++) {
                    QList<Volley> volleyList;
                    QJsonArray jsonArray = jsonArrowList.at(archerIndex).toArray();
                    foreach (QJsonValue jsonVolley, jsonArray) volleyList << Volley(jsonVolley.toArray());
                    arrowList << volleyList;
                }

                emit incomingMatchVolleyList(matchId, arrowList);
                response.insert("response", 0);

                emit incomingMessage((quint32)_socket.peerAddress().toIPv4Address(), targetId, batteryLevel, matchId, "Match reçu");

                break;
            }

            default : {
                qDebug() << "Command not found or value is wrong";
                return reply(-1);

            }
        }


        QJsonDocument doc(response);
        qDebug() << "sending correct response: " << doc.toJson(QJsonDocument::Compact);
        QByteArray byteArray(doc.toJson());
        byteArray.append('\n');
        byteArray.append("EOT\n");   // End Of Transmission

        _socket.write(byteArray);
        _socket.flush();
        _readTimer.start(READ_TIMEOUT);
        return;

    } else {
        qCritical() << "Error while parsing json";
        return;
    }


    return reply(-1);


}
