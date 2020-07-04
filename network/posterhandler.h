#ifndef POSTERHANDLER_H
#define POSTERHANDLER_H

#include <QThread>
#include <QTcpSocket>

class PosterHandler : public QThread {
    Q_OBJECT
public:
    explicit PosterHandler();
    virtual ~PosterHandler();

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

#endif // POSTERHANDLER_H
