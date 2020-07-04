#ifndef TEAM_H
#define TEAM_H

#include <QObject>
#include "archers/archer.h"
#include "teamrule.h"
#include "teamscore.h"

class Team : public QObject {

    Q_OBJECT

public:
    explicit Team(const QString& name, const QList<Archer *>& archerList, QObject *parent = 0);

    void addArcher(Archer* archer);
    TeamScore score(const QString &teamCateg, const QList<TeamRule>& ruleList, int heatIndex = 1, int volleyIndex = 10) const;
    QString name() const;
    QList<Archer*> archerList() const;

private:

    QList<Archer*> _archerList;
    QString _name;

    static int _rankingHeatIndex;
    static int _rankingVolleyIndex;
    static bool scoreRankingLessThan(Archer* archer0, Archer* archer1);

};

#endif // TEAM_H
