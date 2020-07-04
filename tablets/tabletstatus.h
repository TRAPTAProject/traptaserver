#ifndef TABLETSTATUS_H
#define TABLETSTATUS_H

#include <QAbstractTableModel>
#include "tabletinfo.h"

class TabletStatus : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit TabletStatus(QObject *parent = 0);
    
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

signals:
    
public slots:

    void updateTabletStatus(quint32 ip, int targ, int battery, QString message);
    void clear();

private:

    QList<TabletInfo*> _tabletList;
    
};

#endif // TABLETSTATUS_H
