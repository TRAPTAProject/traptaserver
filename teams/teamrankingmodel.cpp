#include "teamrankingmodel.h"
#include <QDebug>
#include <QModelIndex>
#include "sortingalgo.h"
#include <QJsonArray>
#include <QJsonObject>

int TeamRankingModel::_rankingHeatIndex = 0;
int TeamRankingModel::_rankingVolleyIndex = 0;


TeamRankingModel::TeamRankingModel(Database *db, QObject *parent) :
    QAbstractTableModel(parent),
    _composerModel(db, parent),
    _heatIndex(0),
    _volleyIndex(0)
{

}

TeamComposerModel* TeamRankingModel::composerModel() {
    return &_composerModel;
}

// map with categ name (rule map), inside: array of team with rank, name, score
QJsonObject TeamRankingModel::toJson(int heatIndex, int volleyIndex) const {

    QHash<QString, QList<TeamScore> > rankingMap;
    QHash<QString, QList<TeamRule> > ruleMap = _composerModel.ruleMap();
    // create lists for each rule name (team category)
    foreach (QString ruleName, ruleMap.keys()) {
        // for each club, make a team
        QList<TeamScore> teamScoreList;
        foreach (QString club, _clubMap.keys()) {
            Team team(club, *(_clubMap.value(club)));
            // get the score
            TeamScore teamScore = team.score(ruleName, ruleMap.value(ruleName), heatIndex, volleyIndex);
            if (teamScore.score()>0) teamScoreList << teamScore;
        }
        // sort teams
        qSort(teamScoreList.begin(), teamScoreList.end(), teamRankingLessThan);
        rankingMap.insert(ruleName, teamScoreList);
    }

    QJsonObject root;
    foreach (QString categ, rankingMap.keys()) {
        QList<TeamScore> list = rankingMap.value(categ);
        QJsonArray jsonArray;
        for (int index=0; index<list.count(); index++) {
            TeamScore teamScore = list[index];
            QJsonObject jsonTeam;
            jsonTeam.insert("rank", index+1);
            jsonTeam.insert("name", teamScore.name());
            jsonTeam.insert("score", teamScore.score());
            jsonTeam.insert("archerList", teamScore.archerNameList().join("<br>"));
            jsonArray.append(jsonTeam);
        }
        root.insert(categ, jsonArray);
    }

    return root;
}

QStringList TeamRankingModel::getRankingByCateg(const QString& categ) const {
    QStringList list;
    QList<TeamScore> ranking = _rankingMap.value(categ);
    foreach (TeamScore team, ranking) {
        list << team.name();
    }

    return list;
}


void TeamRankingModel::rebuildRanking(const QHash<QString, QList<Archer*>* >& clubMap) {

    qDebug() << "building team ranking";
    if (_flatList.count()>0) removeRows(0, _flatList.count());
    _rankingMap.clear();
    _flatList.clear();
    _clubMap = clubMap;

    QHash<QString, QList<TeamRule> > ruleMap = _composerModel.ruleMap();

    // create lists for each rule name (team category)
    foreach (QString ruleName, ruleMap.keys()) {
        // for each club, make a team
        QList<TeamScore> teamScoreList;
        foreach (QString club, clubMap.keys()) {
            Team team(club, *(clubMap.value(club)));
            // get the score for the maximum heatindex and volleyIndex
            TeamScore teamScore = team.score(ruleName, ruleMap.value(ruleName));
            if (teamScore.score()>0) teamScoreList << teamScore;
        }
        // sort teams
        qSort(teamScoreList.begin(), teamScoreList.end(), teamRankingLessThan);
        _flatList.append(teamScoreList);
        _rankingMap.insert(ruleName, teamScoreList);
    }
    if (_flatList.count()>0) insertRows(0, _flatList.count());

}

bool TeamRankingModel::removeRows(int row, int count, const QModelIndex & parent) {
    beginRemoveRows(parent, row, row+count-1);
    endRemoveRows();
    return true;
}

bool TeamRankingModel::insertRows(int row, int count, const QModelIndex & parent) {
    beginInsertRows(parent, row, row+count-1);
    endInsertRows();
    return true;
}

int TeamRankingModel::rowCount(const QModelIndex& parent) const {
    return _flatList.count();
}

int TeamRankingModel::columnCount(const QModelIndex& parent) const {
    // categ name, team, archers, score
    return 4;

}

QVariant TeamRankingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role!=Qt::DisplayRole) return QVariant();
    if (orientation==Qt::Vertical) {
        if (_flatList.count()<1) return QVariant();
        int counter = 1;
        QString categ = _flatList[0].categ();
        for (int row=0; row<_flatList.count(); row++) {
            if (_flatList[row].categ()!=categ) {
                categ = _flatList[row].categ();
                counter = 1;
            }
            if (row==section) return QVariant(counter);
            counter++;
        }
        return QVariant();
    }
    switch (section) {
        case 0: return QVariant("Catégorie d'équipe");
        case 1: return QVariant("Équipe");
        case 2: return QVariant("Archers marquants");
        case 3: return QVariant("Score");
    }

    return QVariant();

}

QVariant TeamRankingModel::data(const QModelIndex& index, int role) const {
    if (role==Qt::TextAlignmentRole) return QVariant(Qt::AlignLeft);
    if (role!=Qt::DisplayRole) return QVariant();
    int row = index.row();
    int col = index.column();
    if (row>=_flatList.count()) return QVariant();

    switch (col) {
        case 0: return QVariant(_flatList[row].categ());
        case 1: return QVariant(_flatList[row].name());
        case 2: return QVariant(_flatList[row].archerNameList().join(", "));
        case 3: return QVariant(_flatList[row].score());
    }
    return QVariant();

}

bool TeamRankingModel::teamRankingLessThan(TeamScore team0, TeamScore team1) {
    return (team0.score()>team1.score());
}

