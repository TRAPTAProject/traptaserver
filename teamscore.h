#ifndef TEAMSCORE_H
#define TEAMSCORE_H

#include <QObject>
#include "archer.h"

class TeamScore : public QObject
{
    Q_OBJECT
public:
    explicit TeamScore(const QString& categ, const QString& name, const QList<Archer*>& archerList, int heatIndex, int volleyIndex, int score, QObject *parent = 0);
    TeamScore(const TeamScore &copy);
    TeamScore& operator = (const TeamScore& copy);

    int score() const;
    QString categ() const;
    QString name() const;
    QList<Archer*> archerList() const;
    QStringList archerNameList() const;
    int heatIndex() const;
    int volleyIndex() const;

private :

    QString _name;
    QString _categ;
    QList<Archer*> _archerList;
    int _score;
    int _heatIndex;
    int _volleyIndex;


};

#endif // TEAMSCORE_H
