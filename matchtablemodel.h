#ifndef MATCHTABLEMODEL_H
#define MATCHTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "match.h"

class DataModel;

#define MATCHTABLE_COLUMN_COUNT 10

class MatchTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:


    enum {
        TargetColumn = 0,
        CategoryColumn,
        RankAColumn,
        PlayerAColumn,
        ScoreAColumn,
        ScoreBColumn,
        PlayerBColumn,
        RankBColumn,
        ModeColumn,
        TrispotColumn,

    };

    explicit MatchTableModel(int tabId,
                             const QList<Match *> &matchList,
                             DataModel* model,
                             QObject *parent = 0);

    Match* matchAt(int index);

    // reimplemented from QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    Qt::ItemFlags flags(const QModelIndex & index) const;

    bool addMatchList(QList<Match*> matchList);
    bool removeMatchRow(int row);
    QList<Archer*> archerComboList(int row, int playerIndex);

    void toggleTrispot(int rowIndex);

    void dataUpdated(int matchId);

    ~MatchTableModel();

private:

    static bool sortTargetLessThan(Match* match0, Match* match1);
    static bool sortTargetGreaterThan(Match* match0, Match* match1);

    void checkOverlapTarget();

    QList<Match*> _matchList;
    DataModel* _model;

    int _tabId;
    QSet<int> _overlapBucket; // overlapping positions

    QVariant dataForDisplayRole(Match* match, int col) const;
    QVariant dataForTextAlignmentRole(Match* match, int col) const;
    QVariant dataForUserRole(Match* match, int col) const;
    QVariant dataForDecorationRole(Match* match, int col) const;
    QVariant dataForBackgroundRole(Match* match, int col) const;


};

#endif // MATCHTABLEMODEL_H
