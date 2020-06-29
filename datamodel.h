#ifndef ARCHERMAP_H
#define ARCHERMAP_H

#include <QHash>
#include "archer.h"
#include <QMutex>
#include <QTimer>
#include "match.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "team.h"
#include "teamrankingmodel.h"
#include "teamcomposermodel.h"
#include "archertablemodel.h"
#include "matchtablemodel.h"
#include "db.h"
#include <QTextStream>
#include <QFile>

class DataModel : public QObject {
    Q_OBJECT
public:

    enum {
        DisplayDocPosition = 0,
        DisplayDocRanking,
        DisplayDocMatch
    };

    explicit DataModel(DB* sqdb);
    
    Archer* getArcherById(int id);
    Archer* getArcherByPosition(int position);
    QHash<int, QList<Archer*> > getTargetMap(int startId) const;

    // return a ranking list of archer for a startId an categ. startId=0 means all startId
    QList<Archer*> getArcherListSortedByScore(int shootId = 0, const QString& categ = QString()) const;

    // if startId==0, then return s all the start phases, if categ is empty, return all categ, also return the list of teams
    QList<Archer*> getArcherListSortedByName(int shootId = 0, const QString& categ = QString()) const;
    // archer list sorted by position/ if shootId==0 then all archer are returned soprted out by shootId first
    QList<Archer*> getArcherListSortedByPosition(int shootId = 0) const;
    QHash<QString, QList<Archer *> *> getArcherListByCateg() const;
    QHash<int, Archer *> getArcherMap() const;

    QStringList getMatchCategList();  // return the list of categories for matches

    int archerCount(int shootId);
    void loadStringList(const QStringList &getArcherList);
    Archer* addNewArcher(int shootId);
    void clearPointsByShootId(int shootId);
    void deleteArcherByShootId(int shootId);
    void clearArcherList();  // delete all archers + data
    bool restore(const QString& filename);
    void deleteArcherById(int archerId); // delete archer archerId
    void reset();

    void setCategoryList(const QString& filename);
    void setCutList(const QString &filename);

    // JSON stuff
    QJsonObject jsonForTargetList() const;
    QJsonObject jsonForArrowList(const QList<int> &) const;
    QJsonObject jsonForMatchList() const;
   

    void setCurrentShootId(int shootId);
    int currentShootId() const;

    int currentTabId() const;
    void setCurrentTabId(int tabId);

    void setRankingIndex(int heatIndex, int volleyIndex);
    int getWorkingMode();
    void updateDrawCounting();

    void updateArcher(int archerId); // this is called when one of the archer properties has changed (e.g the category. Trigger ranking and build side maps again)

    QColor categHue(const QString& categ);
    bool addMatchList(QList<Match*>matchList);
    bool deleteMatch(int matchId);
    bool deleteMatchTab(int id);
    bool deleteMatchCateg(int tabId, const QString& categ);
    // return -1 if not found
    QMap<int, QString> getMatchTabMap();
    QHash<int, Match*> getMatchMap();
    int addMatchRound(const QString& matchRound); // return roundId
    QList<Match*> getMatchList(int tabId, const QString &categ = QString()) const;
    QSet<QString> getMatchCategSet(int tabId); // get list of categ used
    void setMatchMode(int tabId, const QString& categ, int matchMode); // match in set if setMode is true, point otherwise. Set the mode for all match of type categ


    void setCurrentDisplayTabId(int tabId);
    void setDisplayDocumentSelection(int selection);

    QJsonDocument getDisplayDocument();

    // compute _alphaArcherList and emit it if necessary
    void publishPositionList();

    // global settings
    void setHeatArrowCount(int value);
    void setMatchArrowCount(int value);
    int heatArrowCount() const;
    int matchArrowCount() const;
    int heatVolleyCount() const;
    int matchVolleyMax() const;
    bool heat10X() const;
    int roundCount() const;

    bool publishTeamRanking();
    bool ignorePreviousShoot();

    TeamRankingModel* teamRankingModel();
    ArcherTableModel* archerTableModel(int roundId);
    MatchTableModel* matchTableModel(int roundId);


signals:
    
    // emitted after an incoming volley has been processed
    void dataModelChanged(int archerId);
    void matchModelChanged(int matchId);
    void tabletMessage(quint32 ip, int target, int batteryLevel, QString message);
    void publishDisplayStringList(QStringList stringList);
    void publishDisplay(QJsonDocument doc);
    void inferedVolleyStatusChanged(int heatIndex, int volleyCount);


public slots:

     void processIncomingHeatVolleyList(int id, int heatIndex, const QList<Volley>& list);
     void processIncomingMatchVolleyList(int matchId, const QList<QList<Volley> > &list);
     void processIncomingMatchWinner(int matchId, int winner);
     void processIncomingMessage(quint32 ip, int target, int batteryLevel, int itemId, QString message);
     void setWorkingMode(int workingMode);
     void updateMatchRanking();
     void setHeatVolleyCount(int value);
     void setMatchVolleyMax(int value);
     void setPublishTeamRanking(bool value);
     void setIgnorePreviousShoot(bool value);
     void rebuildSideMaps();

     void processArcherEdited(int archerId);


     void set10XForHeat(bool value);
     void setRoundCount(int roundCount);


private slots:

    void updateRanking2();


private:

    // draw params
    int _roundCount;
    int _heatArrowCount;
    int _matchArrowCount;
    int _heatVolleyCount;
    int _matchVolleyMax;
    bool _x10ForHeat;

    int _idCounter;
    DB* _sqdb;
    int _currentShootId;  // current shoot (starts at 1)
    int _currentTabId; // current match heatScoreCard // starts at 0
    int _currentDisplayTabId;
    int _currentInferedVolleyCount;
    int _currentInferedHeatIndex;
    bool _publishTeamRanking;
    bool _ignorePreviousShoot;

    QJsonDocument _positionDocument;
    QJsonDocument _matchDocument;
    QJsonDocument _rankingDocument;

    TeamRankingModel _teamRankingModel;

    QHash<QString, QString> _categFullName;
    QStringList _categOrderList;
    QMap<int, QString> _tabMap;
    QHash<int, Match*> _matchMap;
    QHash<QString, QColor> _categHue;

    // Map archers with archers id
    QHash<int, Archer*> _archerMap;
    QList<ArcherTableModel*> _archerTableModelList;
    QHash<int, MatchTableModel*> _matchTableModelMap;

    // Map archers with category
    QHash<QString, QList<Archer*>* > _categMap;
    QHash<QString, QList<Archer*>* > _clubMap;

    QMutex _mutex;

    QTimer _rankingTimer;
    QTimer _matchRankingTimer;

    int _rankingHeatIndex;
    int _rankingVolleyIndex;
    int _displayDocumentSelection;

    int _workingMode; //0=qualif, 1=match

    QStringList _categoryAcronymList;  // ordered list of the categories when displaying
    QHash<QString, QString> _categoryPrettyList;   // order list of the category (pretty name) when displaying
    QHash<QString, int> _cutList; // list of categories with cut

    QTextStream* _msgLog;
    QFile* _msgFile;

    void matchArcherName(); // init matches with archer names = id
    void loadMatchTab();
    void reloadMatchList();
    void reloadArcherList();
    void updateRankingWithDelay();
    void updateMatchRankingWithDelay();
    void addToSideMaps(Archer* archer);
    int inferVolleyCount(int shootId, int heatIndex);
    void inferVolleyStatus();

    int findMatchId();



    
};

#endif // ARCHERMAP_H
