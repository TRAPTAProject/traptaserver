#include "datamodel.h"
#include <QStringList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "utils/util.h"
#include <QMutexLocker>
#include <QApplication>
#include "archers/match.h"
#include <QSettings>
#include "archers/heatscorecard.h"
#include "utils/sortingalgo.h"
#include "utils/foldercompressor.h"
#include <QTime>

#define RANKING_TIMER_VALUE 5000 // msec : ranking is published every RANKING_TIMER_VALUE
#define ROUND_COUNT 4

DataModel::DataModel(Database* sqdb) :
    _sqdb(sqdb),
    _teamRankingModel(sqdb, this),
    _msgFile(0),
    _msgLog(0)

{

    _idCounter = 1;
    _rankingTimer.setSingleShot(true);
    _rankingTimer.setInterval(RANKING_TIMER_VALUE);
    _matchRankingTimer.setSingleShot(true);
    _matchRankingTimer.setInterval(RANKING_TIMER_VALUE);

    connect(_teamRankingModel.composerModel(), SIGNAL(itemChanged(QStandardItem*)), SLOT(rebuildSideMaps()));

    _publishTeamRanking = false;
    _rankingHeatIndex = 0;
    _rankingVolleyIndex = 0;
    _currentTabId = 0;
    _workingMode = 0;
    _displayDocumentSelection = 0;
    _currentShootId = 1;
    connect(&_rankingTimer, SIGNAL(timeout()), SLOT(updateRanking2()));
    connect(&_matchRankingTimer, SIGNAL(timeout()), SLOT(updateMatchRanking()));

    QSettings settings;
    _ignorePreviousShoot = settings.value("ignorePreviousShoot", false).toBool();
    _publishTeamRanking = settings.value("publishTeamRanking", false).toBool();
    _heatArrowCount = settings.value("heatArrowCount", 3).toInt();
    _matchArrowCount = settings.value("matchArrowCount", 3).toInt();
    _heatVolleyCount = settings.value("heatVolleyCount", 10).toInt();
    _matchVolleyMax = settings.value("matchVolleyMax", 5).toInt();
    Match::maxVolleyCount = _matchVolleyMax;
    _x10ForHeat = settings.value("x10ForHeat", false).toBool();
    _roundCount = settings.value("roundCountA", 2).toInt();
    HeatScoreCard::x10 = _x10ForHeat;

    // load matchTab list from db
    loadMatchTab();
    // load archer list
    reloadArcherList();
    // load matches
    reloadMatchList();

    _msgFile = new QFile(Util::msgLogFilePath);
    if (_msgFile && _msgFile->open(QIODevice::WriteOnly | QIODevice::Text)) _msgLog = new QTextStream(_msgFile);

}

ArcherTableModel* DataModel::archerTableModel(int roundId) {
    if (roundId>0 && roundId<=_archerTableModelList.count())
        return _archerTableModelList.at(roundId-1);
    return 0;
}

MatchTableModel* DataModel::matchTableModel(int roundId) {
    return _matchTableModelMap.value(roundId);
}

QHash<int, Archer*> DataModel::getArcherMap() const {
    return _archerMap;
}

TeamRankingModel* DataModel::teamRankingModel() {
    return &_teamRankingModel;
}

void DataModel::updateArcher(int archerId) {

}

void DataModel::reloadMatchList() {
    // delete all match
    qDeleteAll(_matchMap);
    _matchMap.clear();

    // delete archer table model
    qDeleteAll(_matchTableModelMap);
    _matchTableModelMap.clear();

    _matchMap = _sqdb->matchMap();
    matchArcherName();
    // for each roundId, create MatchTableModel
    foreach (int roundId, _tabMap.keys()) {
        QList<Match*> matchList;
        foreach (Match* match, _matchMap.values()) {
            if (match->tabId()==roundId) matchList.append(match);
        }
        _matchTableModelMap.insert(roundId, new MatchTableModel(roundId, matchList, this));
    }
    updateMatchRanking();

}

void DataModel::loadMatchTab() {
    _tabMap = _sqdb->matchTabMap();
}

QHash<QString, QList<Archer*>* > DataModel::getArcherListByCateg() const {
    return _categMap;
}

QJsonDocument DataModel::getDisplayDocument() {
    QMutexLocker locker(&_mutex);
    switch (_displayDocumentSelection) {
        case DisplayDocPosition: return _positionDocument;
        case DisplayDocRanking: return _rankingDocument;
        case DisplayDocMatch: return _matchDocument;
    }

    QJsonDocument doc;
    return doc;
}


void DataModel::setDisplayDocumentSelection(int selection) {
    _displayDocumentSelection=selection;

}

void DataModel::publishPositionList() {

    QJsonObject root;
    root.insert("dataType", QString("position"));
    root.insert("title", QString("DEPART %0").arg(_currentShootId));
    QJsonArray jsonArray;
    QList<Archer*> list = getArcherListSortedByName(_currentShootId);
    foreach (Archer* archer, list) jsonArray.append(archer->jsonForPosition());
    root.insert("archerList", jsonArray);
    QJsonDocument jsonDoc(root);
    _positionDocument = jsonDoc;
    //qDebug() << jsonDoc.toJson();
    if (_displayDocumentSelection==DisplayDocPosition) emit publishDisplay(_positionDocument);
}

void DataModel::setCurrentShootId(int shootId) {
    _currentShootId = shootId;
    publishPositionList();

}

int DataModel::currentShootId() const {
    return _currentShootId;
}

int DataModel::archerCount(int shootId) {
    if (shootId==0) return _archerMap.size();
    int count = 0;
    foreach (Archer* archer, _archerMap)
        if (archer->shootId()==shootId) count++;
    return count;
}

// used for match combo populating
QStringList DataModel::getMatchCategList() {
    QStringList list;
    list << _categMap.keys();
    list << _teamRankingModel.composerModel()->ruleMap().keys();
    return list;
}

void DataModel::processArcherEdited(int archerId) {
    rebuildSideMaps();
}

Archer* DataModel::addNewArcher(int shootId) {
    // this a manual add
    // look for available id
    int id = -1;
    foreach (int key, _archerMap.keys()) {
        if (key<=id) id = key-1;
    }
    Archer* archer = _sqdb->createArcher(id, shootId);
    if (archer!=0) {
        qDebug() << "adding new archer: " <<  archer->toString();
        _archerMap.insert(archer->id(), archer);
        rebuildSideMaps();
    }
    connect(archer, SIGNAL(archerEdited(int)), SLOT(processArcherEdited(int)));
    return archer;
}


void DataModel::addToSideMaps(Archer* archer) {

    // categ map
    QList<Archer*>* list1 = _categMap.value(archer->categ());
    if (list1==0) {
        list1 = new QList<Archer*>();
        _categMap.insert(archer->categ(), list1);
    }
    list1->append(archer);

    // club map (only if archer is teammate)
    if (!archer->teammate()) return;
    QList<Archer*>* list2 = _clubMap.value(archer->noc());
    if (list2==0) {
        list2 = new QList<Archer*>();
        _clubMap.insert(archer->noc(), list2);
    }
    list2->append(archer);
}

QColor DataModel::categHue(const QString& categ) {
    if (!_categHue.contains(categ)) return QColor(255,255,255);
    return _categHue.value(categ);
}

void DataModel::rebuildSideMaps() {
    qDeleteAll(_categMap);
    _categMap.clear();
    qDeleteAll(_clubMap);
    _clubMap.clear();

    foreach (Archer* archer, _archerMap) {
        addToSideMaps(archer);
    }
    updateDrawCounting();
    _teamRankingModel.rebuildRanking(_clubMap);
    publishPositionList();
    updateRankingWithDelay();
    updateMatchRankingWithDelay();

    // build color categ
    QStringList categList = getArcherListByCateg().keys();
    categList.sort();
    _categHue.clear();
    int index=0;
    foreach (QString categ, categList) {
        int hue = (250*index++) / categList.count();
        QColor color;
        color.setHsl(hue, 220, 220);
        _categHue.insert(categ, color);
    }


}

QHash<int, QList<Archer*> > DataModel::getTargetMap(int shootId) const {
    QHash<int, QList<Archer*> > targetMap;
    foreach (Archer* archer, _archerMap) {
        if (archer->shootId()!=shootId) continue;
        if (archer->position()<1) continue;
        int targetId = Util::targetIdFromPosition(archer->position());
        if (targetId<1) continue;
        QList<Archer*> list = targetMap.value(targetId);
        list.append(archer);
        targetMap.insert(targetId, list);
    }
    return targetMap;
}

QList<Archer*> DataModel::getArcherListSortedByScore(int shootId, const QString& categ) const {
    QList<Archer*> list;
    qDebug() << "Looking for archer categ: " << categ << " with startId " << shootId;
    foreach (Archer* archer, _archerMap.values()) {
        //qDebug() << "archer " << archer->getStartId() << " " << archer->getCateg();
        if ((archer->shootId()==shootId || shootId<1) &&
                (categ.isEmpty() || categ==archer->categ()))
                list.append(archer);
    }
    // sort the list
    std::sort(list.begin(), list.end(), SortingAlgo::scoreFullRankingLessThan);

    return list;
}

// returns the archer at position "position" for startId==currentStartId
Archer* DataModel::getArcherByPosition(int position) {
    QList<Archer*> list;
    foreach (Archer* archer, _archerMap.values())
        if (archer->shootId()==_currentShootId && archer->position()==position) return archer;
    return 0;
}

QList<Archer*> DataModel::getArcherListSortedByName(int shootId, const QString& categ) const {

    QList<Archer*> list;
    foreach (Archer* archer, _archerMap.values())
        if ((archer->shootId()==shootId || shootId<1) &&
                (categ.isEmpty() || categ==archer->categ()))
                    list.append(archer);

    // sort the list
    std::sort(list.begin(), list.end(), SortingAlgo::alphaRankingLessThan);

    return list;
}

QList<Archer*> DataModel::getArcherListSortedByPosition(int shootId) const {

    QList<Archer*> list;
    if (shootId<1) {
        list = _archerMap.values();
        std::sort(list.begin(), list.end(), SortingAlgo::shootIdPositionRankingLessThan);
    } else {
        foreach (Archer* archer, _archerMap.values())
            if (archer->shootId()==shootId) list.append(archer);
        // sort the list
        std::sort(list.begin(), list.end(), SortingAlgo::positionRankingLessThan);
    }
    return list;
}


// return the list of archer for a given shoot
QJsonObject DataModel::jsonForTargetList() const {
    qDebug() << "Retrieving json Target List";
    QJsonObject jsonObject;
    QHash<int, QList<Archer*> > map = getTargetMap(_currentShootId);
    foreach (int targetId, map.keys()) {
        QJsonArray jsonArray;
        foreach (Archer* archer, map.value(targetId)) {
            jsonArray.append(archer->jsonForTablet());
        }
        jsonObject.insert(QString::number(targetId), jsonArray);
    }
    return jsonObject;

}

void DataModel::setCurrentTabId(int tabId) {
    _currentTabId = tabId;
}

int DataModel::currentTabId() const {
    return _currentTabId;
}

void DataModel::setCurrentDisplayTabId(int tabId) {
    _currentDisplayTabId = tabId;
    updateMatchRanking();
}

void DataModel::setWorkingMode(int workingMode) {
    // tab 0 and 1 are for working mode 0 (qualif), tab 2 is for matches

    if (workingMode==0 || workingMode==1) _workingMode = 0;
    else _workingMode = 1;

    qDebug() << "Setting workingMode to " << _workingMode;
}

int DataModel::getWorkingMode() {
    return _workingMode;
}

// return the list of match for a given tab (heatScoreCard)
QJsonObject DataModel::jsonForMatchList() const {
    qDebug() << "Retrieving json MatchRow List";
    QJsonObject jsonObject;
    // get all matches from current tab and for all categ ("")
    QList<Match*> matchList = getMatchList(_currentTabId);
    foreach (Match* match, matchList) {
        if (match->targetId()>0) jsonObject.insert(QString::number(match->targetId()), match->jsonDataSet());
    }
    return jsonObject;

}

// list of arrows for archers whose ids are passed as parameters (sent to tablets)
QJsonObject DataModel::jsonForArrowList(const QList<int>& list) const {
    QJsonObject jsonObject;
    foreach (int id, list) {
        Archer* archer = _archerMap.value(id);
        if (archer!=0) jsonObject.insert(QString::number(id), archer->jsonArrowList());
    }
    return jsonObject;

}

void DataModel::clearPointsByShootId(int shootId) {
    qDebug() << "Delete points for shootId=" << shootId;
    _sqdb->clearPointsByShootId(shootId);
    foreach (Archer* archer, _archerMap.values()) {
        for (int i=0; i<ARCHER_MAX_HEAT; i++) archer->resetScoreCard(i);
        emit dataModelChanged(archer->id());  // this is to update UI
    }
    inferVolleyStatus();
    updateRankingWithDelay();
}

void DataModel::clearArcherList() {

    bool ok = _sqdb->clearArcherList();
    reloadArcherList();

}

void DataModel::reset() {
    _sqdb->reset();
    _matchMap.clear();
    _tabMap.clear();
    reloadArcherList();
}

void DataModel::deleteArcherById(int archerId) {
    qDebug() << "Deleting archer id " << archerId;
   _sqdb->deleteArcherById(archerId);
   Archer* archer = _archerMap.take(archerId);
   delete archer;
   rebuildSideMaps();
   updateDrawCounting();
   updateRanking2();
}

void DataModel::deleteArcherByShootId(int shootId) {
    qDebug() << "Deleting archers for shootId=" << shootId;
    _sqdb->deleteArcherByShootId(shootId);
    //_dbFactory->deleteArcherListById(idList);
    reloadArcherList();
}

void DataModel::updateDrawCounting() {
    qDebug() << "Updating draw counters";
    QMutexLocker locker(&_mutex);
    QList<Archer*> list =_archerMap.values();
    int archerCount = list.size();

    for (int refIndex=0; refIndex<archerCount; refIndex++) {
        if (list.isEmpty()) break;
        Archer* refArcher = list.takeFirst();
        refArcher->setDrawId(1);
        //qDebug() << "checking draw for archer: " << refArcher->toString();
        QList<Archer*> foundList;
        foundList << refArcher;
        foreach (Archer* compareArcher, list) {
            if (refArcher->sameAs(compareArcher)) foundList << compareArcher;
        }
        if (foundList.size()<2) continue;
        // now if same archers were found, update draw and remove them from the list
        int draw = 1;
        int found = foundList.size();
        QList<Archer*> foundListCopy = foundList;
        for (int i=0; i<found; i++) {
            int shootId=999;
            Archer* archer = 0;
            // look for smallest startId
            foreach (Archer* foundArcher, foundList) {
                if (foundArcher->shootId()<shootId) {
                    shootId = foundArcher->shootId();
                    archer = foundArcher;
                }
            }
            if (archer!=0) {
                archer->setDrawId(draw++);
                foundList.removeOne(archer);
            }

        }
        // remove all the archers already processed (contained in foundList)
        foreach (Archer* foundArcher, foundListCopy) list.removeOne(foundArcher);

    }


}

QMap<int, QString> DataModel::getMatchTabMap() {
    return _tabMap;
}

QHash<int, Match*> DataModel::getMatchMap() {
    return _matchMap;
}

void DataModel::setMatchMode(int tabId, const QString& categ, int matchMode) {
    QList<Match*> list = getMatchList(tabId, categ);
    foreach (Match* match, list) {
        // check mode before settings because it triggers an access to db
        if (match->mode()!=matchMode) match->setMode(matchMode);
    }
}

QList<Match*> DataModel::getMatchList(int tabId, const QString& categ) const {
    QList<Match*> list;
    foreach (Match* matchModel, _matchMap.values()) {
        if (matchModel->tabId()==tabId && (categ.isEmpty() || matchModel->categ()==categ)) list.append(matchModel);
    }
    return list;
}

QSet<QString> DataModel::getMatchCategSet(int tabId) {
    QSet<QString> set;
    foreach (Match* matchModel, _matchMap.values()) {
        if (matchModel->tabId()==tabId) set.insert(matchModel->categ());
    }
    return set;
}

bool DataModel::deleteMatchTab(int id) {
    bool ok = _sqdb->deleteMatchTab(id);
    if (ok) _tabMap.remove(id);
    return ok;
}

bool DataModel::deleteMatchCateg(int tabId, const QString& categ) {
    bool ok = _sqdb->deleteMatchCateg(tabId, categ);
    if (ok) {
        reloadMatchList();
    }
    return ok;
}

bool DataModel::deleteMatch(int matchId) {

    bool ok = _sqdb->deleteMatchById(matchId);
    if (ok) _matchMap.remove(matchId);
    return ok;

}

bool DataModel::addMatchList(QList<Match*> matchList) {
    QList<int> idList = _sqdb->insertMatchList(matchList);
    if (idList.count()!=matchList.count()) {
        qCritical() << "Something went wrong while inserting matches. abort.";
        return false;
    }
    int matchIndex = 0;
    foreach (Match* match, matchList) {
        match->setDB(_sqdb);
        int id = idList.at(matchIndex++);
        match->setId(id);
        _matchMap.insert(id, match);
    }
    matchArcherName();
    return true;
}

int DataModel::addMatchRound(const QString& matchRound) {
    int tabId = _sqdb->insertMatchTab(matchRound);
    if (tabId>-1) {
        _tabMap.insert(tabId, matchRound);
        _matchTableModelMap.insert(tabId, new MatchTableModel(tabId, QList<Match*>(), this));
    }
    return tabId;
}

int DataModel::inferVolleyCount(int shootId, int heatIndex) {

    int array[HEATSCORECARD_MAX_VOLLEY];
    for (int i=0; i<HEATSCORECARD_MAX_VOLLEY; i++) array[i] = 0;
    foreach (Archer* archer, getArcherListSortedByName(shootId)) {
        int n = archer->volleyCount(heatIndex);
        if (n>-1 && n<HEATSCORECARD_MAX_VOLLEY) array[n]++;
    }
    // now get the greatest number for volley
    int max=0;
    int volleyCount=0;
    for (int i=0; i<HEATSCORECARD_MAX_VOLLEY; i++) {
        if (array[i]>max) {
            max = array[i];
            volleyCount = i;
        }
    }
    return volleyCount;
}

void DataModel::inferVolleyStatus() {
    int oldInferedHeatIndex = _currentInferedHeatIndex;
    int oldInferedVolleyCount = _currentInferedVolleyCount;
    // if current display startId is 0 then _currentInferedVolleyIndex = -1 (no meaning)
    if (_currentShootId<1) {
        _currentInferedVolleyCount = -1;
        _currentInferedHeatIndex = -1;
        return;
    }

    _currentInferedHeatIndex=0;
    _currentInferedVolleyCount=0;
    // infer current heat index
    for (int i=ARCHER_MAX_HEAT-1; i>=0; i--) {
        int volleyCount = inferVolleyCount(_currentShootId, i);
        if (volleyCount>0) {
            _currentInferedHeatIndex = i;
            _currentInferedVolleyCount = volleyCount;
            break;
        }
    }

    // set upToDate field for each archer
    foreach (Archer* archer, getArcherListSortedByName(_currentShootId)) {
        // reset warning
        for (int i=0; i<ARCHER_MAX_HEAT; i++) archer->setWarning(i, false);
        int n = archer->volleyCount(_currentInferedHeatIndex);
        if (n!=_currentInferedVolleyCount) archer->setWarning(_currentInferedHeatIndex, true);
    }
    if (oldInferedHeatIndex!=_currentInferedHeatIndex || oldInferedVolleyCount!=_currentInferedVolleyCount) {
        qDebug() << "Most of people have reached heatIndex=" << _currentInferedHeatIndex
                 << " and drew volleyCount=" << _currentInferedVolleyCount;
        emit inferedVolleyStatusChanged(_currentInferedHeatIndex, _currentInferedVolleyCount);
    }


}

void DataModel::reloadArcherList() {
    //SELECT * FROM `archertable` LEFT JOIN `volleytable` on (archertable.id=volleytable.id)
    qDebug() << "Loading archer list from DB";

    // delete all archers
    qDeleteAll(_archerMap);
    _archerMap.clear();

    // delete archer table model
    qDeleteAll(_archerTableModelList);
    _archerTableModelList.clear();

    _archerMap = _sqdb->archerMap();
    // look for max shootId
    int shootCount = 0;
    foreach (Archer* archer, _archerMap.values()) {
        int shootId = archer->shootId();
        connect(archer, SIGNAL(archerEdited(int)), SLOT(processArcherEdited(int)));
        if (shootCount<shootId) shootCount = shootId;
    }
    // for each shoot, create ArcherTableModel
    for (int shoot=1; shoot<=shootCount; shoot++) {
        QList<Archer*> archerList;
        foreach (Archer* archer, _archerMap.values()) {
            if (archer->shootId()==shoot) archerList.append(archer);
        }
        _archerTableModelList.append(new ArcherTableModel(shoot, archerList, _x10ForHeat));
    }

    rebuildSideMaps();
    updateDrawCounting();
    updateRanking2();
    inferVolleyStatus();

}


void DataModel::loadStringList(const QStringList& archerList) {
    //_dbFactory->loadArcherStringList(archerList);
    _sqdb->loadFromStringList(archerList);
    reloadArcherList();
}

void DataModel::processIncomingMatchVolleyList(int matchId, const QList< QList<Volley> >& list) {
    Match* match = _matchMap.value(matchId);
    if (match==0) {
        qDebug() << "There is no match Id " << matchId;
        return;
    }
    if (list.count()<2) {
        qDebug() << "I need 2 lists of volleys to process match volley";
    }
    match->setScoreCard(MatchScoreCard(list.at(0)), MatchScoreCard(list.at(1)));
    emit matchModelChanged(matchId);
    updateMatchRankingWithDelay();
}

void DataModel::processIncomingMatchWinner(int matchId, int winner) {
    Match* match = _matchMap.value(matchId);
    if (match==0) {
        qDebug() << "There is no match Id " << matchId;
        return;
    }
    match->setWinner(winner);
    emit matchModelChanged(matchId);
    updateMatchRankingWithDelay();
}


void DataModel::processIncomingHeatVolleyList(int id, int heatIndex, const QList<Volley>& list) {
    qDebug() << "Processing volley list for archer " << id << " with heatIndex " << heatIndex;
    // https://bitbucket.org/traptateam/traptaserver/issues/1/traitement-de-volees-de-serie-3-ou-4
    // if heatIndex out of bound, for example heatIndex is 2 (3rd round) and the number of rounds is 2, then ignore
    if (heatIndex>=_roundCount) {
        qWarning() << "Heat index " << heatIndex << " is out of bound for archer " << id;
        return;
    }
    // get archer
    Archer* archer = _archerMap.value(id);
    if (archer!=0) {
        archer->setScoreCard(heatIndex, HeatScoreCard(list));
        inferVolleyStatus();
        emit dataModelChanged(id);  // this is to update UI
        updateRankingWithDelay();
        // update team ranking
        _teamRankingModel.rebuildRanking(_clubMap);

    }
    else {
        qDebug() << "There is no archer with id=" << id;
    }
    
}

void DataModel::setRankingIndex(int heatIndex, int volleyIndex) {
    SortingAlgo::_staticRankingHeatIndex = heatIndex;
    SortingAlgo::_staticRankingVolleyIndex = volleyIndex;
    _rankingHeatIndex = heatIndex;
    _rankingVolleyIndex = volleyIndex;
    updateRankingWithDelay();
    // update team ranking
    _teamRankingModel.rebuildRanking(_clubMap);
}

void DataModel::updateMatchRankingWithDelay() {
    _matchRankingTimer.start();
}

void DataModel::updateRankingWithDelay() {
    _rankingTimer.start();
}

void DataModel::updateMatchRanking() {
    qDebug() << "Updating match ranking";
    // get the list of matches for the current tab
    QList<Match*> list = getMatchList(_currentDisplayTabId);
    std::sort(list.begin(), list.end(), SortingAlgo::matchTargetLessThan);
    QJsonObject root;
    root.insert("dataType", QString("matches"));
    root.insert("title", _tabMap.value(_currentDisplayTabId, "?"));
    QJsonArray jsonArray;
    foreach (Match* match, list) {
        if (match->targetId()>0) jsonArray.append(match->jsonForDisplay());
    }
    root.insert("targetList", jsonArray);
    _matchDocument = QJsonDocument(root);
    if (_displayDocumentSelection==DisplayDocMatch) emit publishDisplay(_matchDocument);
}

void DataModel::setCategoryList(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        _categoryAcronymList.clear();
        _categoryPrettyList.clear();
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString string = in.readLine().trimmed();
            if (string.isNull()) break;
            if (string.isEmpty()) continue;
            QStringList list = string.split('=');
            if (list.count()!=2) continue;
            _categoryAcronymList << list[0].trimmed();
            _categoryPrettyList.insert(list[0].trimmed(),list[1].trimmed());
        }
        file.close();
    }
    updateRankingWithDelay();
}

void DataModel::setCutList(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        _cutList.clear();
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString string = in.readLine().trimmed();
            if (string.isNull()) break;
            if (string.isEmpty()) continue;
            QStringList list = string.split('=');
            if (list.count()!=2) continue;
            _cutList.insert(list[0].trimmed(),list[1].trimmed().toInt());
        }
        file.close();
    }
    updateRankingWithDelay();
}

void DataModel::updateRanking2() {
    qDebug() << "Start ranking2";
    QMutexLocker locker(&_mutex);
    QHash<QString, QList<Archer*>* > sortedRankMap = _categMap;  // make a copy of the list
    foreach (QString categ, sortedRankMap.keys()) {
        QList<Archer*>* list = sortedRankMap.value(categ);
        std::sort(list->begin(), list->end(), SortingAlgo::scoreRankingLessThan);
    }

    // determine order of categories
    QStringList categListSource = _categMap.keys();
    QStringList categListDest;
    // for all items in categ list reference
    foreach (QString string, _categoryAcronymList) {
        int index = categListSource.indexOf(string);
        // if categ found
        if (index>-1) {
            categListDest << string;
            categListSource.removeAt(index);
        }
    }
    // now dump the rest of categListSource
    foreach (QString string, categListSource) categListDest << string;

    QJsonObject jsonRoot;
    jsonRoot.insert("dataType", QString("ranking"));
    jsonRoot.insert("heatIndex", _rankingHeatIndex);
    jsonRoot.insert("volleyIndex", _rankingVolleyIndex);
    jsonRoot.insert("arrowCount", _heatArrowCount);
    // if displaying last volley, do not print round phase running
    if (_rankingHeatIndex<ROUND_COUNT-1 || _rankingVolleyIndex<_heatVolleyCount-1) jsonRoot.insert("roundId", _currentShootId);
    QJsonArray jsonCategArray;
    int globalVolleyIndex = _rankingHeatIndex*_heatVolleyCount+_rankingVolleyIndex;
    foreach (QString categ, categListDest) {
        QList<Archer*>* listptr = sortedRankMap.value(categ);
        if (listptr==0) continue;
        QList<Archer*> list = (QList<Archer*>)(*listptr); // make a copy of the list
        if (list.size()==0) continue;
        QJsonArray jsonArray;
        bool runningCateg = false; // tells if this category contains an archer in the currentRun (do not display locally)

        int rank = 0;
        foreach (Archer* archer, list) {
            if (archer->score()<1) continue; // ignore archer with 0 points
            if (_ignorePreviousShoot && archer->shootId()!=_currentShootId) continue; // ignore other startId if asked to
            if (archer->drawId()>1 && archer->shootId()!=_currentShootId) continue; // ignore archer if not first draw and not currently shooting
            bool currentRun = false;
            if (archer->shootId()==_currentShootId) {
                currentRun = true;
                runningCateg = true;
            }

            QString rankStr = "--";
            if (archer->drawId()==1) {
                rank++;
                archer->setRank(globalVolleyIndex, rank);
                rankStr = QString::number(rank);
            }
            QJsonObject jsonItem;
            jsonItem.insert("rank", rankStr);
            jsonItem.insert("currentRun", currentRun);
            bool includeTrend = true;
            if (_rankingVolleyIndex==_heatVolleyCount-1) includeTrend = false; // do not include trend if last volley
            if (!currentRun) includeTrend = false; // do not include trend if not current run
            jsonItem.insert("archer", archer->jsonForRanking(_rankingHeatIndex, _rankingVolleyIndex, globalVolleyIndex, includeTrend, currentRun));
            jsonArray.append(jsonItem);
        }
        if (jsonArray.count()>0) {
            QJsonObject jsonCategObj;
            QString categName = _categoryPrettyList.value(categ);
            if (categName.isEmpty()) categName = categ;
            jsonCategObj.insert("categName", categName);
            jsonCategObj.insert("runningCateg", runningCateg);
            jsonCategObj.insert("cut", _cutList.value(categ));
            jsonCategObj.insert("archerList", jsonArray);
            jsonCategArray.append(jsonCategObj);
        }

    }
    jsonRoot.insert("ranking", jsonCategArray);
    if (_publishTeamRanking) jsonRoot.insert("team", _teamRankingModel.toJson(_rankingHeatIndex, _rankingVolleyIndex));
    _rankingDocument = QJsonDocument(jsonRoot);
    // emit string list if necessary
    if (_displayDocumentSelection==DisplayDocRanking) emit publishDisplay(_rankingDocument);
    qDebug() << "Finish ranking2";

}

void DataModel::processIncomingMessage(quint32 ip, int target, int batteryLevel, int itemId, QString message) {

    // log event
    Archer* archer = getArcherById(itemId);
    if (archer && _msgLog) {
        *_msgLog << QString("%0 - Adresse %1, Cible %2, Batterie %3%, Archer %4, %5\n").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(ip & 0xFF).arg(target).arg(batteryLevel).arg(archer->name()).arg(message);
        _msgLog->flush();
    }
    if (itemId==0 && _msgLog) {
        *_msgLog << QString("%0 - Adresse %1, Cible %2, Batterie %3%, Données récupérées\n").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(ip & 0xFF).arg(target).arg(batteryLevel);
        _msgLog->flush();
    }

    emit tabletMessage(ip, target, batteryLevel, message);
}

Archer* DataModel::getArcherById(int id) {
    return _archerMap.value(id);
}

void DataModel::setHeatArrowCount(int value) {
    QSettings settings;
    _heatArrowCount = value;
    settings.setValue("heatArrowCount", value);
}

void DataModel::setMatchArrowCount(int value) {
    QSettings settings;
    _matchArrowCount = value;
    settings.setValue("matchArrowCount", value);
}

void DataModel::setHeatVolleyCount(int value) {
    qDebug() << "Setting heatVolleyCount to " << value;
    QSettings settings;
    _heatVolleyCount = value;
    settings.setValue("heatVolleyCount", value);
}

void DataModel::setMatchVolleyMax(int value) {
    QSettings settings;
    _matchVolleyMax = value;
    Match::maxVolleyCount = value;
    settings.setValue("matchVolleyMax", value);
}

void DataModel::setRoundCount(int roundCount) {
    _roundCount = roundCount;
    QSettings settings;
    settings.setValue("roundCountA", _roundCount);
}

int DataModel::roundCount() const {
    return _roundCount;
}

void DataModel::set10XForHeat(bool value) {
    QSettings settings;
    _x10ForHeat = value;
    HeatScoreCard::x10 = value;
    foreach (ArcherTableModel* tableModel, _archerTableModelList) {
        tableModel->set10X(value);
    }
    settings.setValue("x10ForHeat", value);
}

void DataModel::setIgnorePreviousShoot(bool value) {
    QSettings settings;
    _ignorePreviousShoot = !value;
    settings.setValue("ignorePreviousShoot", _ignorePreviousShoot);
    updateRanking2();
}

void DataModel::setPublishTeamRanking(bool value) {
    QSettings settings;
    _publishTeamRanking = value;
    settings.setValue("publishTeamRanking", value);
    updateRanking2();
}

bool DataModel::ignorePreviousShoot() {
    return _ignorePreviousShoot;
}

bool DataModel::publishTeamRanking() {
    return _publishTeamRanking;
}


int DataModel::heatArrowCount() const {
    return _heatArrowCount;
}

int DataModel::matchArrowCount() const {
    return _matchArrowCount;
}

int DataModel::heatVolleyCount() const {
    return _heatVolleyCount;
}

int DataModel::matchVolleyMax() const {
    return _matchVolleyMax;
}

bool DataModel::heat10X() const {
    return _x10ForHeat;
}

void DataModel::matchArcherName() {
    // for each match find archer names
    foreach (Match* match, _matchMap.values()) {
        for (int i=0; i<2; i++) {
            int archerId = match->archerId(i);
            Archer* archer = _archerMap.value(archerId);
            if (archer!=0) match->setArcherName(i, archer->name());
        }
    }
}


bool DataModel::restore(const QString& filename) {
    //_dbFactory->clear();
//    FolderCompressor compressor;
//    compressor.decompressFolder(filename, Util::workingDir);
//    //_dbFactory->open();
//    // load matchTab list from db
//    loadMatchTab();
//    // load archer list
//    reloadArcherList();
//    // load matches
//    loadMatchFromDB();
    return true;
}
