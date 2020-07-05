#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "utils/util.h"
#include <QSqlDriver>
#include "archers/volley.h"

#define ID_INDEX 0
#define LICENSE_INDEX 1
#define CATEG_INDEX 2
#define LASTNAME_INDEX 3
#define FIRSTNAME_INDEX 4
#define CLUB_INDEX 5
#define SHOOT_INDEX 6
#define POSITION_INDEX 7
#define TRISPOT_INDEX 8

#define COLUMN_COUNT 9

/*
CREATE TABLE archer(`archerId` INTEGER PRIMARY KEY, `position` INTEGER, `license` VARCHAR(100), `category` VARCHAR(10), `noc` VARCHAR(200), `firstname` VARCHAR(100), `lastname` VARCHAR(100), trispot BOOLEAN, teammate BOOLEAN, shootId INTEGER);
CREATE TABLE heatVolley(`volleyId` INTEGER PRIMARY KEY AUTOINCREMENT, `archerId` INTEGER, `heatIndex` INTEGER, `volleyIndex` INTEGER, arrow0 INTEGER, arrow1 INTEGER, arrow2 INTEGER, arrow3 INTEGER, arrow4 INTEGER, arrow5 INTEGER);
CREATE TABLE match(`matchId` INTEGER PRIMARY KEY AUTOINCREMENT, `tabId` INTEGER, `category` VARCHAR(10), `round` INTEGER, `rank` INTEGER, `manual` BOOLEAN, `position` INTEGER, `trispot` BOOLEAN, `tieBreak` INTEGER, `mode` INTEGER, `player0` INTEGER, `player1` INTEGER);
CREATE TABLE teamrule(`ruleId` INTEGER PRIMARY KEY AUTOINCREMENT, `ruleName` VARCHAR(200), `ruleExpression` VARCHAR(300));
CREATE TABLE matchVolley(`volleyId` INTEGER PRIMARY KEY AUTOINCREMENT, `matchId` INTEGER, `playerIndex` INTEGER, `volleyIndex` INTEGER, arrow0 INTEGER, arrow1 INTEGER, arrow2 INTEGER, arrow3 INTEGER, arrow4 INTEGER, arrow5 INTEGER);
CREATE TABLE matchTab(`tabId` INTEGER PRIMARY KEY AUTOINCREMENT, `name` VARCHAR(100));
*/

Database::Database(const QString& dbfile, QObject *parent) : QObject(parent),
     _open(false)
{
    // Create db
    qDebug() << "Using " << dbfile << " as sqlite DB";
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(dbfile);
    _db.open();

    if (!_db.isOpen()) {
        qCritical() << "Database is not open !";
        qCritical() << _db.lastError().text();
    }
    else {
        _open = true;
    }


}

// return -1 if something goes wrong
int Database::insertMatchTab(const QString& roundName) {

    QString sql = QString("INSERT INTO `matchTab` (`name`) values ('%0')").arg(roundName);
    QSqlQuery query(_db);
    bool ok = query.exec(sql);
    if (!ok) {
        qWarning() << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();

}

bool Database::setRuleMap(const QList<QStringList>& list) {
    if (list.count()<2) return false;
    QStringList nameList = list.at(0);
    QStringList expressionList = list.at(1);
    if (nameList.count()!=expressionList.count()) return false;
    QString sql = "DELETE FROM `teamrule`";

    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }


    QSqlQuery deleteQuery(_db);
    deleteQuery.exec(sql);
    QSqlQuery query(_db);
    query.prepare("INSERT INTO `teamrule` (`ruleName`,`ruleExpression`) values (?,?)");
    int index = 0;

    foreach (QString name, nameList) {
        QString expression = expressionList.at(index++);
        query.bindValue(0, name);
        query.bindValue(1, expression);
        bool result = query.exec();
        if (!result) {
            qWarning() << "Cannot insert rule with name " << name;
            qWarning() << query.lastError().text();
            _db.rollback();
            return false;
        }
    }
    _db.commit();
    return true;
}

QList<QStringList> Database::ruleMap() {
    QList<QStringList> list;
    QStringList nameList;
    QStringList expressionList;
    QString sql = "SELECT * FROM `teamrule` ORDER BY `ruleId` ASC";
    QSqlQuery query(_db);
    query.exec(sql);
    while (query.next()) {
        nameList << query.value("ruleName").toString();
        expressionList << query.value("ruleExpression").toString();
    }
    list << nameList;
    list << expressionList;
    return list;
}

QMap<int, QString> Database::matchTabMap() {
    QMap<int, QString> map;
    QString sql = "SELECT * FROM `matchTab` ORDER BY `tabId` ASC";
    QSqlQuery query(_db);
    query.exec(sql);
    while (query.next()) {
        int tabId = query.value("tabId").toInt();
        QString name = query.value("name").toString();
        map.insert(tabId, name);
    }
    return map;
}

QHash<int, Match*> Database::matchMap() {
    QHash<int, Match*> map;
    QString sql = "SELECT * FROM `match` LEFT JOIN `matchVolley` ON (match.matchId=matchVolley.matchId) ORDER  BY `matchId`, `playerIndex`, `volleyIndex` ASC";
    QSqlQuery query(_db);
    query.exec(sql);

    int currentMatchId = -9999;
    Match* currentMatch = 0;
    QList<Volley> volleyList[2];

    while (query.next()) {
        bool ok;
        int matchId = query.value("matchId").toInt(&ok);
        if (!ok) {
            qWarning() << "Cannot convert " << query.value("matchId").toString() << " to integer. Ignore row.";
            continue;
        }
        // change match
        if (matchId!=currentMatchId) {
            if (currentMatch!=0) {
                currentMatch->setScoreCard(MatchScoreCard(volleyList[0]), MatchScoreCard(volleyList[1]), false);
                map.insert(currentMatch->id(), currentMatch);
            }
            currentMatchId = matchId;
            int targetId = query.value("position").toInt();
            int tabId = query.value("tabId").toInt();
            QString category = query.value("category").toString();
            int round = query.value("round").toInt();
            int rank = query.value("rank").toInt();
            bool trispot = query.value("trispot").toBool();
            bool manual = query.value("manual").toBool();
            int tieBreakWinner = query.value("tieBreak").toInt();
            int mode = query.value("mode").toInt();
            int player0 = query.value("player0").toInt();
            int player1 = query.value("player1").toInt();
            currentMatch = new Match(matchId,
                                     tabId,
                                     category,
                                     round,
                                     rank,
                                     manual,
                                     targetId,
                                     trispot,
                                     tieBreakWinner,
                                     mode, player0, player1,
                                     this);

            volleyList[0].clear();
            volleyList[1].clear();
        }
        int playerIndex = query.value("playerIndex").toInt(&ok);
        if (!ok) {
            qWarning() << "No playerIndex for match " << matchId << ". No arrow value.";
            continue;
        }
        QList<int> arrowList;
        arrowList << query.value("arrow0").toInt();
        arrowList << query.value("arrow1").toInt();
        arrowList << query.value("arrow2").toInt();
        arrowList << query.value("arrow3").toInt();
        arrowList << query.value("arrow4").toInt();
        arrowList << query.value("arrow5").toInt();
        volleyList[playerIndex].append(Volley(arrowList));

    }
    if (currentMatch!=0) {
        currentMatch->setScoreCard(MatchScoreCard(volleyList[0]), MatchScoreCard(volleyList[1]), false);
        map.insert(currentMatch->id(), currentMatch);
    }

    return map;

}

bool Database::deleteMatchById(int matchId) {
    QString sql = QString("DELETE FROM `match` WHERE `matchId`=%0").arg(matchId);
    QSqlQuery query(_db);
    bool ok = query.exec(sql);
    if (!ok) {
        qWarning() << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::deleteMatchCateg(int roundId, const QString& categ) {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QString sql = QString("SELECT * FROM `match` WHERE (`tabId`=%0 AND `category`='%1')").arg(roundId).arg(categ);
    QSqlQuery query(_db);
    QSqlQuery deleteQuery(_db);
    query.exec(sql);
    while (query.next()) {
        int matchId = query.value("matchId").toInt();
        deleteQuery.exec(QString("DELETE FROM `matchVolley` WHERE `matchId`=%0").arg(matchId));
    }
    bool ok = query.exec(QString("DELETE FROM `match` WHERE (`tabId`=%0 AND `category`='%1')").arg(roundId).arg(categ));
    if (!ok) {
        qCritical() << query.lastError().text();
        _db.rollback();
        return false;
    }

    _db.commit();
    return true;
}

bool Database::deleteMatchTab(int roundId) {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QString sql = QString("SELECT * FROM `match` WHERE `matchId`=%0").arg(roundId);
    QSqlQuery query(_db);
    QSqlQuery deleteQuery(_db);
    query.exec(sql);
    while (query.next()) {
        int matchId = query.value("matchId").toInt();
        deleteQuery.exec(QString("DELETE FROM `matchVolley` WHERE `matchId`=%0").arg(matchId));
    }
    bool ok = query.exec(QString("DELETE FROM `match` WHERE `tabId`=%0").arg(roundId));
    if (!ok) {
        qCritical() << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec(QString("DELETE FROM `matchTab` WHERE `tabId`=%0").arg(roundId));
    if (!ok) {
        qCritical() << query.lastError().text();
        _db.rollback();
        return false;
    }
    _db.commit();
    return true;
}

// returns ids of matches
QList<int> Database::insertMatchList(QList<Match*> matchList) {
    QList<int> idList;
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QSqlQuery query(_db);
    query.prepare("INSERT INTO `match` (`tabId`,`category`,`round`,`rank`, `manual`, `position`, `trispot`, `tieBreak`, `mode`, `player0`, `player1`) values (?,?,?,?,?,?,?,?,?,?,?)");
    foreach (Match* match, matchList) {
        query.bindValue(0, match->tabId());
        query.bindValue(1, match->categ());
        query.bindValue(2, match->round());
        query.bindValue(3, match->rank());
        query.bindValue(4, match->manualRank()?1:0);
        query.bindValue(5, match->targetId());
        query.bindValue(6, match->trispot()?1:0);
        query.bindValue(7, match->tieBreak());
        query.bindValue(8, match->mode());
        query.bindValue(9, match->archerId(0));
        query.bindValue(10, match->archerId(1));
        bool result = query.exec();
        if (!result) {
            qWarning() << "Cannot insert match with player 0 " << match->archerId(0);
            qWarning() << query.lastError().text();
            break;
        }
        idList << query.lastInsertId().toInt();
    }

    _db.commit();
    return idList;
}



// load archers info + scorecards
QHash<int, Archer*> Database::archerMap() {
    QHash<int, Archer*> map;
    QString sql = "SELECT * FROM `archer` LEFT JOIN `heatVolley` ON (archer.archerId=heatVolley.archerId) ORDER  BY `shootId`, `archerId`, `heatIndex`, `volleyIndex` ASC";
    QSqlQuery query(_db);
    query.exec(sql);

    int currentArcherId = -9999;
    int currentHeatIndex = -1;
    Archer* currentArcher = 0;
    QList<Volley> currentVolleyList;
    while (query.next()) {
        bool ok;
        int archerId = query.value("archerId").toInt(&ok);
        if (!ok) {
            qWarning() << "Cannot convert " << query.value("archerId").toString() << " to integer. Ignore row.";
            continue;
        }
        if (archerId!=currentArcherId) {
            currentArcherId = archerId;
            if (currentHeatIndex>-1) {
                currentArcher->setScoreCard(currentHeatIndex, HeatScoreCard(currentVolleyList), false);
            }
            if (currentArcher!=0) map.insert(currentArcher->id(), currentArcher);
            int position = query.value("position").toInt();
            QString license = query.value("license").toString();
            QString category = query.value("category").toString();
            QString noc = query.value("noc").toString();
            QString firstname = query.value("firstname").toString();
            QString lastname = query.value("lastname").toString();
            bool trispot = query.value("trispot").toBool();
            bool teammate = query.value("teammate").toBool();
            int shootId = query.value("shootId").toInt();
            currentArcher = new Archer(archerId, license, category, firstname, lastname, noc, shootId, position, trispot, teammate, this);
            currentHeatIndex = -1;
        }
        int heatIndex = query.value("heatIndex").toInt(&ok);
        if (!ok) {
            qWarning() << "No heatIndex for archer " << archerId << ". No arrow value.";
            continue;
        }
        if (currentHeatIndex!=heatIndex && currentArcher!=0) { // change heat
            if (currentHeatIndex>-1) {
                currentArcher->setScoreCard(currentHeatIndex, HeatScoreCard(currentVolleyList), false);
            }
            currentHeatIndex = heatIndex;
            currentVolleyList.clear();
        }
        QList<int> arrowList;
        arrowList << query.value("arrow0").toInt();
        arrowList << query.value("arrow1").toInt();
        arrowList << query.value("arrow2").toInt();
        arrowList << query.value("arrow3").toInt();
        arrowList << query.value("arrow4").toInt();
        arrowList << query.value("arrow5").toInt();
        currentVolleyList.append(Volley(arrowList));


    }
    if (currentHeatIndex>-1) {
        currentArcher->setScoreCard(currentHeatIndex, HeatScoreCard(currentVolleyList), false);
    }
    if (currentArcher!=0) map.insert(currentArcher->id(), currentArcher);
    return map;

}

Archer* Database::createArcher(int id, int shootId) {

    Archer* archer = new Archer(id, "?", "?", "?", "?", "?", shootId, 0, false, false, this);
    QString sql = QString("INSERT INTO `archer` (`archerId`,`position`,`license`,`category`, `noc`, `firstname`, `lastname`, `trispot`, `teammate`, `shootId`) values (%0,0,'?','?','?','?','?',0,0,%1)")
            .arg(id)
            .arg(shootId);
    QSqlQuery query(_db);
    bool ok = query.exec(sql);
    if (!ok) {
        qWarning() << query.lastError().text();
        return 0;
    }
    return archer;
}

bool Database::updateMatch(int matchId,
                 int position,
                 bool trispot,
                 int tieBreak,
                 int mode,
                 int player0,
                 int player1)
{
    QString sql = QString("UPDATE `match` SET `position`='%0', `trispot`='%1', `tieBreak`='%2', `mode`='%3', `player0`='%4', `player1`='%5' WHERE `matchId`='%6'")
            .arg(position)
            .arg(trispot)
            .arg(tieBreak)
            .arg(mode)
            .arg(player0)
            .arg(player1)
            .arg(matchId);

    qDebug() << "Updating match: " << sql;
    QSqlQuery query(_db);
    bool ok = query.exec(sql);
    if (!ok) {
        qWarning() << query.lastError().text();
        return false;
    }
    return true;

}


bool Database::updateArcher(int id,
                  const QString& license,
                  const QString& categ,
                  const QString& firstname,
                  const QString& lastname,
                  const QString& noc,
                  int position,
                  bool trispot, bool teammate)
{

    QString sql = QString("UPDATE `archer` SET `position`='%0', `license`='%1', `category`='%2', `noc`='%3', `firstname`='%4', `lastname`='%5', `trispot`='%6', `teammate`='%7' WHERE `archerId`='%8'")
            .arg(position)
            .arg(license)
            .arg(categ)
            .arg(noc)
            .arg(firstname)
            .arg(lastname)
            .arg(trispot)
            .arg(teammate)
            .arg(id);
    qDebug() << "Updating archer: " << sql;
    QSqlQuery query(_db);
    bool ok = query.exec(sql);
    if (!ok) {
        qWarning() << query.lastError().text();
        return false;
    }
    return true;

}

bool Database::reset() {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QSqlQuery query(_db);
    bool ok = query.exec("DELETE FROM `heatVolley`");
    if (!ok) {
        qCritical() << "Cannot clear all data in heatVolley: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec("DELETE FROM `archer`");
    if (!ok) {
        qCritical() << "Cannot clear all archers: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec("DELETE FROM `match`");
    if (!ok) {
        qCritical() << "Cannot clear all archers: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec("DELETE FROM `matchTab`");
    if (!ok) {
        qCritical() << "Cannot clear all archers: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec("DELETE FROM `matchVolley`");
    if (!ok) {
        qCritical() << "Cannot clear all archers: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec("DELETE FROM `teamrule`");
    if (!ok) {
        qCritical() << "Cannot clear all teamrules: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    _db.commit();
    return true;
}

bool Database::clearPointsByShootId(int shootId) {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QString sql = QString("SELECT * FROM `archer` WHERE `shootId`=%0").arg(shootId);
    QSqlQuery query(_db);
    QSqlQuery deleteQuery(_db);
    query.exec(sql);
    while (query.next()) {
        int archerId = query.value("archerId").toInt();
        deleteQuery.exec(QString("DELETE FROM `heatVolley` WHERE `archerId`=%0").arg(archerId));
    }
    _db.commit();
    return true;
}

bool Database::clearPointsByArcherId(int id, int roundIndex) {
     QSqlQuery query(_db);
     QString sql = QString("DELETE FROM `heatVolley` WHERE (`archerId`=%0 AND `heatIndex`=%1)").arg(id).arg(roundIndex);
     qDebug() << sql;
     bool ok = query.exec(sql);
     if (!ok) {
         qCritical() << "Cannot delete points for archer with id " << id << ":" << query.lastError().text();
         return false;
     }
     return true;
}



bool Database::deleteArcherById(int id) {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QSqlQuery query(_db);
    QString sql = QString("DELETE FROM `archer` WHERE `archerId`=%0").arg(id);
    qDebug() << sql;
    bool ok = query.exec(sql);
    if (!ok) {
        qCritical() << "Cannot delete archer with id " << id << ":" << query.lastError().text();
        _db.rollback();
        return false;
    }
    sql = QString("DELETE FROM `heatVolley` WHERE `archerId`=%0").arg(id);
    qDebug() << sql;
    ok = query.exec(sql);
    if (!ok) {
        qCritical() << "Cannot delete archer with id " << id << ":" << query.lastError().text();
        _db.rollback();
        return false;
    }
    _db.commit();
    return true;
}


bool Database::deleteArcherByShootId(int shootId) {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QString sql = QString("SELECT * FROM `archer` WHERE `shootId`=%0").arg(shootId);
    QSqlQuery query(_db);
    QSqlQuery deleteQuery(_db);
    query.exec(sql);
    while (query.next()) {
        int archerId = query.value("archerId").toInt();
        deleteQuery.exec(QString("DELETE FROM `heatVolley` WHERE `archerId`=%0").arg(archerId));
    }
    bool ok = query.exec(QString("DELETE FROM `archer` WHERE `shootId`=%0").arg(shootId));
    if (!ok) {
        qCritical() << query.lastError().text();
        _db.rollback();
        return false;
    }
    _db.commit();
    return true;
}

bool Database::clearArcherList() {
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QSqlQuery query(_db);
    bool ok = query.exec("DELETE FROM `heatVolley`");
    if (!ok) {
        qCritical() << "Cannot clear all data in heatVolley: " << query.lastError().text();
        _db.rollback();
        return false;
    }
    ok = query.exec("DELETE FROM `archer`");
    if (!ok) {
        qCritical() << "Cannot clear all archers: " << query.lastError().text();
        _db.rollback();
        return false;
    }

    _db.commit();
    return true;
}

// this deletes and insert a new volley list
bool Database::updateHeatScoreCard(int archerId, int heatIndex, QList<Volley> volleyList) {
    // first delete any existing score card
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QSqlQuery deleteQuery(_db);
    QString sql = QString("DELETE FROM `heatVolley` WHERE (`archerId`='%0' AND `heatIndex`=%1)").arg(archerId).arg(heatIndex);
    bool ok = deleteQuery.exec(sql);
    if (!ok) {
        qCritical() << "Cannot delete score card " << deleteQuery.lastError().text();
        _db.rollback();
        return false;
    }
    QSqlQuery insertQuery(_db);
    insertQuery.prepare(QString("INSERT INTO `heatVolley` (`archerId`,`heatIndex`,`volleyIndex`,`arrow0`, `arrow1`, `arrow2`, `arrow3`, `arrow4`, `arrow5`) values ('%0','%1',?,?,?,?,?,?,?)")
            .arg(archerId)
            .arg(heatIndex));
    int volleyIndex = 0;
    foreach (Volley volley, volleyList) {
        insertQuery.bindValue(0, volleyIndex++);
        QList<int> arrowList = volley.arrowList();
        int arrowIndex = 1;
        foreach (int arrow, arrowList)
            insertQuery.bindValue(arrowIndex++, arrow);
        bool result = insertQuery.exec();
        if (!result) {
            qWarning() << "Cannot update scorecard for archer id " << archerId;
            qWarning() << insertQuery.lastError().text();
            _db.rollback();
            return false;
        }
    }
    _db.commit();
    return true;
}

// this deletes and insert a new volley list
bool Database::updateMatchScoreCard(int matchId, QList< QList<Volley> > tabList) {
    if (tabList.count()!=2) return false;
    // first delete any existing score card
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }
    QSqlQuery deleteQuery(_db);
    QString sql = QString("DELETE FROM `matchVolley` WHERE (`matchId`='%0')").arg(matchId);
    bool ok = deleteQuery.exec(sql);
    if (!ok) {
        qCritical() << "Cannot delete score card " << deleteQuery.lastError().text();
        _db.rollback();
        return false;
    }
    QSqlQuery insertQuery(_db);
    insertQuery.prepare(QString("INSERT INTO `matchVolley` (`matchId`,`playerIndex`,`volleyIndex`,`arrow0`, `arrow1`, `arrow2`, `arrow3`, `arrow4`, `arrow5`) values ('%0',?,?,?,?,?,?,?,?)")
            .arg(matchId));


    for (int playerIndex=0; playerIndex<2; playerIndex++) {
        QList<Volley> volleyList = tabList[playerIndex];
        int volleyIndex = 0;
        insertQuery.bindValue(0, playerIndex);
        foreach (Volley volley, volleyList) {
            insertQuery.bindValue(1, volleyIndex++);
            QList<int> arrowList = volley.arrowList();
            int arrowIndex = 2;
            foreach (int arrow, arrowList)
                insertQuery.bindValue(arrowIndex++, arrow);
            bool result = insertQuery.exec();
            if (!result) {
                qWarning() << "Cannot update scorecard for match id " << matchId;
                qWarning() << insertQuery.lastError().text();
                _db.rollback();
                return false;
            }
        }

    }

    _db.commit();
    return true;
}


bool Database::loadFromStringList(const QStringList& stringlist) {
    qDebug() << "Loading archer list from string list";
    if (!_db.transaction()) {
        qWarning() << "Could not start transaction";
    }

    QSqlQuery query(_db);
    query.prepare("INSERT INTO `archer` (`archerId`,`position`,`license`,`category`, `noc`, `firstname`, `lastname`, `trispot`, `teammate`, `shootId`) values (?,?,?,?,?,?,?,?,?,?)");

    int counter = 0;
    foreach (QString string, stringlist) {
        counter++;
        QStringList tab;
        // check if the separator is a colon or semi-colon
        if (string.indexOf(';')>-1) tab = string.split(';');
        else tab = string.split(',');
        int shift = 0; // shift = -1 if not from resultarc
        if (tab.size()>=COLUMN_COUNT) shift = 0; // from result'arc
        else if (tab.size()==COLUMN_COUNT-1) shift = -1; // from spreadsheet
        else {
            qDebug() << "Too few values for string: " << string;
            return false;
        }

        bool ok;
        // license
        QString license = Util::normalize(tab[LICENSE_INDEX+shift].trimmed());
        if (license.isEmpty()) {
            qDebug() << "Licence is empty in " << string;
            continue;
        }
        QString category = Util::normalize(tab[CATEG_INDEX+shift].trimmed());
        QString firstname = Util::normalize(tab[FIRSTNAME_INDEX+shift].trimmed());
        QString lastname = Util::normalize(tab[LASTNAME_INDEX+shift].trimmed());
        QString club = Util::normalize(tab[CLUB_INDEX+shift].trimmed());
        int shootId = tab[SHOOT_INDEX+shift].trimmed().toInt(&ok);
        if (!ok) shootId = 1;
        int position = 0;
        QString positionStr = tab[POSITION_INDEX+shift].trimmed();
        position = Util::positionFromTargetLabel(positionStr);
        bool trispot = (tab[TRISPOT_INDEX+shift].toInt(&ok)==1);
        if (!ok) trispot = false;

        int id = 0;
        if (shift==0) {  // data from resultarc
            id = tab[ID_INDEX].trimmed().toInt(&ok);
            if (!ok) {
                qDebug() << "Cannot parse id in " << string;
                continue;
            }
        }
        else { // data manually built
            id = 1000*shootId+counter;
        }

        query.bindValue(0, id);
        query.bindValue(1, position);
        query.bindValue(2, license);
        query.bindValue(3, category);
        query.bindValue(4, club);
        query.bindValue(5, firstname);
        query.bindValue(6, lastname);
        query.bindValue(7, trispot);
        query.bindValue(8, false);
        query.bindValue(9, shootId);
        bool result = query.exec();
        if (!result) {
            qWarning() << "Cannot insert archer with licence " << license;
            qWarning() << query.lastError().text();
        }
    }
    _db.commit();
    return true;
}
