#ifndef TEAMRANKINGMODEL_H
#define TEAMRANKINGMODEL_H

#include <QAbstractTableModel>
#include "team.h"
#include <QJsonObject>
#include "teamcomposermodel.h"

class TeamRankingModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TeamRankingModel(DB* db, QObject *parent = 0);

    void rebuildRanking(const QHash<QString, QList<Archer*>* >& clubMap);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    QJsonObject toJson(int heatIndex, int volleyIndex) const;
    TeamComposerModel* composerModel();

    QStringList getRankingByCateg(const QString& categ) const;

private:

    // Map team category with list of team
    QHash<QString, QList<TeamScore> > _rankingMap; // for each catgory rule, list of team, sorted
    QHash<QString, QList<Archer*>* > _clubMap;
    QList<TeamScore> _flatList;
    int _volleyIndex;
    int _heatIndex;
    TeamComposerModel _composerModel;

    static bool teamRankingLessThan(TeamScore team0, TeamScore team1);
    static int _rankingHeatIndex;
    static int _rankingVolleyIndex;

};

#endif // TEAMRANKINGMODEL_H
