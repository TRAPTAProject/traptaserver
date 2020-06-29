#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "archers/archer.h"
#include "archers/volley.h"
#include "archers/match.h"
#include <QList>

class Archer;
class Match;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString& dbfile, QObject *parent = 0);

    QHash<int, Match*> matchMap();
    QList<QStringList> ruleMap();
    bool setRuleMap(const QList<QStringList>& list);
    Archer* createArcher(int id, int shootId);
    QList<int> insertMatchList(QList<Match*> matchList);
    int insertMatchTab(const QString& roundName);
    QMap<int, QString> matchTabMap();
    bool deleteMatchTab(int roundId);
    bool deleteMatchCateg(int roundId, const QString& categ);
    bool deleteMatchById(int matchId);

    QHash<int, Archer*> archerMap();
    bool updateArcher(int id,
                      const QString& license,
                      const QString& categ,
                      const QString& firstname,
                      const QString& lastname,
                      const QString& noc,
                      int position,
                      bool trispot, bool teammate);

    bool updateMatch(int matchId,
                     int position,
                     bool trispot,
                     int tieBreak,
                     int mode,
                     int player0,
                     int player1);

    bool deleteArcherById(int id);
    bool deleteArcherByShootId(int shootId);
    bool reset();
    bool clearPointsByShootId(int shootId);
    bool clearPointsByArcherId(int archerId, int roundIndex);
    bool clearArcherList();


    // this deletes and insert a new volley list
    bool updateHeatScoreCard(int archerId, int heatIndex, QList<Volley> volleyList);
    bool updateMatchScoreCard(int matchId, QList< QList<Volley> > volleyList);
    bool loadFromStringList(const QStringList& stringlist);


private:

    QSqlDatabase _db;
    bool _open;

};

#endif // DATABASE_H
