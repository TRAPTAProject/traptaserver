#ifndef ARCHERTABLEMODEL_H
#define ARCHERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include "archers/archer.h"
#include <QSet>

#define ARCHERTABLE_COLUMN_COUNT 15

class ArcherTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:


    enum {
        PositionColumn = 0,
        LicenseColumn,
        CategoryColumn,
        NameColumn,
        NocColumn,
        TrispotColumn,
        TeammateColumn,
        Round1Column,
        Round2Column,
        Round3Column,
        Round4Column,
        ScoreColumn,
        Criteria1Column,
        Criteria2Column,
        DrawIdColumn
    };

    explicit ArcherTableModel(int roundId, const QList<Archer *> &archerList, bool x10, QObject *parent = 0);

    Archer* archerAt(int index);

    // reimplemented from QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    Qt::ItemFlags flags(const QModelIndex & index) const;

    bool addArcher(Archer* archer);
    bool removeArcher(int row);

    void set10X(bool x10);
    void toggleTrispot(int rowIndex);
    void toggleTeammate(int rowIndex);

    void dataUpdated(int archerId);

private:

    static bool sortPositionLessThan(Archer* archer0, Archer* archer1);
    static bool sortPositionGreaterThan(Archer* archer0, Archer* archer1);

    static bool sortCategoryLessThan(Archer* archer0, Archer* archer1);
    static bool sortCategoryGreaterThan(Archer* archer0, Archer* archer1);

    static bool sortLicenseLessThan(Archer* archer0, Archer* archer1);
    static bool sortLicenseGreaterThan(Archer* archer0, Archer* archer1);

    static bool sortNameLessThan(Archer* archer0, Archer* archer1);
    static bool sortNameGreaterThan(Archer* archer0, Archer* archer1);

    static bool sortNOCLessThan(Archer* archer0, Archer* archer1);
    static bool sortNOCGreaterThan(Archer* archer0, Archer* archer1);

    static bool sortScoreLessThan(Archer* archer0, Archer* archer1);
    static bool sortScoreGreaterThan(Archer* archer0, Archer* archer1);

    void checkOverlapPosition();

    QList<Archer*> _archerList;
    int _shootId;
    bool _x10;
    QSet<int> _overlapBucket; // overlapping positions

    QVariant dataForDisplayRole(Archer* archer, int col) const;
    QVariant dataForTextAlignmentRole(Archer* archer, int col) const;
    QVariant dataForUserRole(Archer* archer, int col) const;
    QVariant dataForDecorationRole(Archer* archer, int col) const;
    QVariant dataForBackgroundRole(Archer* archer, int col) const;


};

#endif // ARCHERTABLEMODEL_H
