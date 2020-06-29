#ifndef TABLETINFO_H
#define TABLETINFO_H

#include <QObject>
#include <QStringList>

class TabletInfo : public QObject {

    Q_OBJECT
public:
    TabletInfo(int addr, QObject *parent = 0);

    void addMessage(const QString& msg);
    int addr() const;
    int targ() const;
    int batt() const;
    QString msg() const;

    void setBatt(int batt);
    void setTarg(int targ);

    QString toString() const;


signals:

public slots:

private:

    int _addr;
    int _targ;
    int _batt;
    QString _msg;
    //QStringList _msgList;

};

#endif // TABLETINFO_H
