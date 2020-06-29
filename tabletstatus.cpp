#include "tabletstatus.h"
#include "sortingalgo.h"
#include <QDebug>

TabletStatus::TabletStatus(QObject *parent) :
    QAbstractTableModel(parent) {


}

int TabletStatus::rowCount(const QModelIndex& parent) const {
    return _tabletList.count();
}

int TabletStatus::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant TabletStatus::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role!=Qt::DisplayRole) return QVariant();
    if (orientation==Qt::Vertical) return QVariant(section+1);
    switch (section) {
        case 0: return QVariant("Cibl");
        case 1: return QVariant("Adr");
        case 2: return QVariant("Batt");
        case 3: return QVariant("Message");
    }

    return QVariant();
}

QVariant TabletStatus::data(const QModelIndex& index, int role) const {
    int row = index.row();
    int col = index.column();

    if (role==Qt::TextAlignmentRole) {
        if (col==4) return QVariant(Qt::AlignLeft);
        return QVariant(Qt::AlignCenter);
    }
    TabletInfo* info = _tabletList.at(row);
    if (role==Qt::BackgroundRole && col==2) {
        if (info->batt()>30 | info->batt()<0) return QVariant(QBrush(Qt::white));
        else return QVariant(QBrush(Qt::red));

    }

    if (role!=Qt::DisplayRole) return QVariant();
    if (row>=_tabletList.count()) return QVariant();

    switch (col) {
        case 0: return QVariant(info->targ());
        case 1: return QVariant(info->addr());
        case 2: {
            if (info->batt()>-1) return QVariant(info->batt());
            return QVariant("?");
        }
        case 3: return QVariant(info->msg());
    }

    return QVariant();
}

void TabletStatus::clear() {
    if (_tabletList.count()<1) return;
    removeRows(0, _tabletList.count());
    qDeleteAll(_tabletList);
    _tabletList.clear();


}

bool TabletStatus::removeRows(int row, int count, const QModelIndex & parent) {
    beginRemoveRows(parent, row, row+count-1);

    endRemoveRows();
}

bool TabletStatus::insertRows(int row, int count, const QModelIndex & parent) {
    beginInsertRows(parent, row, row+count-1);

    endInsertRows();

}

void TabletStatus::updateTabletStatus(quint32 ip, int targ, int batt, QString message) {

    int addr = ip & 0xFF;

    TabletInfo* info = 0;
    // find tablet
    foreach (TabletInfo* item, _tabletList)
        if (item->addr()==addr) {
            info = item;
            break;
        }

    if (info==0) {    // tablet does not exist yet
        info = new TabletInfo(addr, this);
        _tabletList.append(info);
        insertRows(0, 1);
    }
    info->setTarg(targ);
    info->setBatt(batt);
    info->addMessage(message);
    qDebug() << info->toString();
    qSort(_tabletList.begin(), _tabletList.end(), SortingAlgo::tabletInfoLessThan);
    emit dataChanged(createIndex(0,0), createIndex(_tabletList.count(), 4));


}


