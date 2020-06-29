#include "archertablemodel.h"
#include <QDebug>
#include <QIcon>
#include "util.h"

ArcherTableModel::ArcherTableModel(int shootId, const QList<Archer*>& archerList, bool x10, QObject *parent) :
    QAbstractTableModel(parent),
    _x10(x10),
    _archerList(archerList),
    _shootId(shootId)

{
    checkOverlapPosition();
}

void ArcherTableModel::set10X(bool x10) {
    _x10 = x10;
    emit headerDataChanged(Qt::Horizontal, Criteria1Column, Criteria2Column);
}

void ArcherTableModel::toggleTrispot(int rowIndex) {
    if (rowIndex<0 || rowIndex>=_archerList.count()) return;
    Archer* archer = _archerList.at(rowIndex);
    archer->setTrispot(!archer->trispot());
    emit dataChanged(createIndex(rowIndex,TrispotColumn), createIndex(rowIndex, TrispotColumn));
}

void ArcherTableModel::toggleTeammate(int rowIndex) {
    if (rowIndex<0 || rowIndex>=_archerList.count()) return;
    Archer* archer = _archerList.at(rowIndex);
    archer->setTeammate(!archer->teammate());
    emit dataChanged(createIndex(rowIndex, TeammateColumn), createIndex(rowIndex, TeammateColumn));
}


Qt::ItemFlags ArcherTableModel::flags(const QModelIndex & index) const {
    int col = index.column();
    if (col>NocColumn) return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
    return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;

}

int ArcherTableModel::rowCount(const QModelIndex &parent) const {
    return _archerList.count();
}

int ArcherTableModel::columnCount(const QModelIndex &parent) const {
    return ARCHERTABLE_COLUMN_COUNT;
}

bool ArcherTableModel::addArcher(Archer* archer) {
    beginInsertRows(QModelIndex(), 0, 0);
    _archerList.prepend(archer);
    endInsertRows();
    emit dataChanged(createIndex(0,0), createIndex(_archerList.count(), ARCHERTABLE_COLUMN_COUNT));
}

bool ArcherTableModel::removeArcher(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    _archerList.removeAt(row);
    endRemoveRows();
    emit dataChanged(createIndex(0,0), createIndex(_archerList.count(), ARCHERTABLE_COLUMN_COUNT));
}


void ArcherTableModel::sort(int column, Qt::SortOrder order) {
    qDebug() << "Sorting column " << column << "order " << order;
    if (column==PositionColumn && order==Qt::AscendingOrder) qSort(_archerList.begin(), _archerList.end(), sortPositionLessThan);
    if (column==PositionColumn && order==1) qSort(_archerList.begin(), _archerList.end(), sortPositionGreaterThan);
    if (column==LicenseColumn && order==Qt::AscendingOrder) qSort(_archerList.begin(), _archerList.end(), sortLicenseLessThan);
    if (column==LicenseColumn && order==Qt::DescendingOrder) qSort(_archerList.begin(), _archerList.end(), sortLicenseGreaterThan);
    if (column==CategoryColumn && order==Qt::AscendingOrder) qSort(_archerList.begin(), _archerList.end(), sortCategoryLessThan);
    if (column==CategoryColumn && order==Qt::DescendingOrder) qSort(_archerList.begin(), _archerList.end(), sortCategoryGreaterThan);
    if (column==NameColumn && order==Qt::AscendingOrder) qSort(_archerList.begin(), _archerList.end(), sortNameLessThan);
    if (column==NameColumn && order==Qt::DescendingOrder) qSort(_archerList.begin(), _archerList.end(), sortNameGreaterThan);
    if (column==NocColumn && order==Qt::AscendingOrder) qSort(_archerList.begin(), _archerList.end(), sortNOCLessThan);
    if (column==NocColumn && order==Qt::DescendingOrder) qSort(_archerList.begin(), _archerList.end(), sortNOCGreaterThan);
    if (column==ScoreColumn && order==Qt::AscendingOrder) qSort(_archerList.begin(), _archerList.end(), sortScoreLessThan);
    if (column==ScoreColumn && order==Qt::DescendingOrder) qSort(_archerList.begin(), _archerList.end(), sortScoreGreaterThan);

    emit dataChanged(createIndex(0,0), createIndex(_archerList.count(), ARCHERTABLE_COLUMN_COUNT));
}

bool ArcherTableModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (role!=Qt::EditRole) return false;
    int row = index.row();
    int col = index.column();
    if (row>=_archerList.count()) {
        qDebug() << "No archer at row " << row;
        return false;
    }
    Archer* archer = _archerList.at(row);
    switch (col) {
        case PositionColumn: {
            archer->setPosition(value.toInt());
            checkOverlapPosition();
            emit dataChanged(createIndex(0, PositionColumn), createIndex(_archerList.count(), PositionColumn));
            return true;
        }
        case LicenseColumn: archer->setLicense(value.toString()); break;
        case CategoryColumn: archer->setCategory(value.toString()); break;
        case NameColumn: archer->setName(value.toString()); break;
        case NocColumn: archer->setNoc(value.toString()); break;
        default: return false;
    }
    emit dataChanged(createIndex(row, col), createIndex(row, col));

    return true;
}

QVariant ArcherTableModel::dataForDisplayRole(Archer *archer, int col) const {
    switch (col) {
        case PositionColumn: return QVariant(Util::targetLabelFromPosition(archer->position()));
        case LicenseColumn: return QVariant(archer->license());
        case CategoryColumn: return QVariant(archer->categ());
        case NameColumn: return QVariant(archer->name());
        case NocColumn: return QVariant(archer->noc());
        case Round1Column: return QVariant(QString("%0/%1").arg(archer->score(0)).arg(archer->volleyCount(0)));
        case Round2Column: return QVariant(QString("%0/%1").arg(archer->score(1)).arg(archer->volleyCount(1)));
        case Round3Column: return QVariant(QString("%0/%1").arg(archer->score(2)).arg(archer->volleyCount(2)));
        case Round4Column: return QVariant(QString("%0/%1").arg(archer->score(3)).arg(archer->volleyCount(3)));
        case ScoreColumn: return QVariant(archer->score());
        case Criteria1Column: return QVariant(archer->criteria1());
        case Criteria2Column: return QVariant(archer->criteria2());
        case DrawIdColumn: return QVariant(archer->drawId());
    }
    return QVariant();
}

QVariant ArcherTableModel::dataForTextAlignmentRole(Archer *archer, int col) const {
    switch (col) {
        case PositionColumn: return QVariant(Qt::AlignCenter);
        case Round1Column: return QVariant(Qt::AlignCenter);
        case Round2Column: return QVariant(Qt::AlignCenter);
        case Round3Column: return QVariant(Qt::AlignCenter);
        case Round4Column: return QVariant(Qt::AlignCenter);
        case ScoreColumn: return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        case Criteria1Column: return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        case Criteria2Column: return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        case DrawIdColumn: return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    return QVariant();
}

QVariant ArcherTableModel::dataForUserRole(Archer *archer, int col) const {
    switch (col) {
        case PositionColumn: return QVariant(archer->position());
        case TeammateColumn: return QVariant(archer->teammate());
        case TrispotColumn: return QVariant(archer->trispot());
    }

    return QVariant();
}

QVariant ArcherTableModel::dataForDecorationRole(Archer *archer, int col) const {
    switch (col) {
        case TrispotColumn: {
            if (archer->trispot()) return QVariant(QIcon(":/checkbox.png"));
            break;
        }
        case TeammateColumn: {
            if (archer->teammate()) return QVariant(QIcon(":/checkbox.png"));
            break;
        }
        case Round1Column: {
            if (archer->warned(0)) return QVariant(QIcon(":/warning-icon.png"));
            break;
        }
        case Round2Column: {
            if (archer->warned(1)) return QVariant(QIcon(":/warning-icon.png"));
            break;
        }
        case Round3Column: {
            if (archer->warned(2)) return QVariant(QIcon(":/warning-icon.png"));
            break;
        }
        case Round4Column: {
            if (archer->warned(3)) return QVariant(QIcon(":/warning-icon.png"));
            break;
        }
    }
    return QVariant();
}

QVariant ArcherTableModel::dataForBackgroundRole(Archer* archer, int col) const {
    if (col==PositionColumn && _overlapBucket.contains(archer->position())) return QVariant(QBrush(QColor(250,0,0)));
    if (col==PositionColumn && archer->position()==0) return QVariant(QBrush(QColor(250,128,0)));
    if (Util::targetIdFromPosition(archer->position())%2==0) return QVariant(QBrush(QColor(128,128,128)));
    return QVariant();
}

QVariant ArcherTableModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int col = index.column();
    if (row>=_archerList.count()) {
        qDebug() << "No archer at row " << row;
        return QVariant();
    }
    Archer* archer = _archerList.at(row);

    switch (role) {
        case Qt::DisplayRole: return dataForDisplayRole(archer, col);
        case Qt::UserRole: return dataForUserRole(archer, col);
        case Qt::TextAlignmentRole: return dataForTextAlignmentRole(archer, col);
        case Qt::DecorationRole: return dataForDecorationRole(archer, col);
        case Qt::BackgroundRole: return dataForBackgroundRole(archer, col);
    }

    return QVariant();

}

QVariant ArcherTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role==Qt::DisplayRole && orientation==Qt::Horizontal) {
        switch (section) {
            case PositionColumn: return QVariant("Position");
            case LicenseColumn: return QVariant("Licence");
            case CategoryColumn: return QVariant("Catégorie");
            case NameColumn: return QVariant("Nom");
            case NocColumn: return QVariant("NOC / Club");
            case TrispotColumn: return QVariant("Trispot");
            case TeammateColumn: return QVariant("Equipier");
            case Round1Column: return QVariant("Série 1");
            case Round2Column: return QVariant("Série 2");
            case Round3Column: return QVariant("Série 3");
            case Round4Column: return QVariant("Série 4");
            case ScoreColumn: return QVariant("Total");
            case Criteria1Column: if (_x10) return QVariant("10X"); else return QVariant("10");
            case Criteria2Column: if (_x10) return QVariant("10"); else return QVariant("9");
            case DrawIdColumn: return QVariant("Tir");
        }
    }
    return QVariant();
}

Archer* ArcherTableModel::archerAt(int index) {
    if (index<0 || index>=_archerList.count()) return 0;
    return _archerList.at(index);
}

bool ArcherTableModel::sortPositionLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->position()<archer1->position());
}

bool ArcherTableModel::sortPositionGreaterThan(Archer* archer0, Archer* archer1) {
    return (archer0->position()>archer1->position());
}

bool ArcherTableModel::sortCategoryLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->categ()<archer1->categ());
}

bool ArcherTableModel::sortCategoryGreaterThan(Archer* archer0, Archer* archer1) {
    return (archer0->categ()>archer1->categ());
}

bool ArcherTableModel::sortLicenseLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->license()<archer1->license());
}

bool ArcherTableModel::sortLicenseGreaterThan(Archer* archer0, Archer* archer1) {
    return (archer0->license()>archer1->license());
}

bool ArcherTableModel::sortNameLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->name()<archer1->name());
}

bool ArcherTableModel::sortNameGreaterThan(Archer* archer0, Archer* archer1) {
    return (archer0->name()>archer1->name());
}

bool ArcherTableModel::sortNOCLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->noc()<archer1->noc());
}

bool ArcherTableModel::sortNOCGreaterThan(Archer* archer0, Archer* archer1) {
    return (archer0->noc()>archer1->noc());
}

bool ArcherTableModel::sortScoreLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->score()<archer1->score());
}

bool ArcherTableModel::sortScoreGreaterThan(Archer* archer0, Archer* archer1) {
    return (archer0->score()>archer1->score());
}

void ArcherTableModel::checkOverlapPosition() {
    _overlapBucket.clear();
    QSet<int> checkBucket;
    foreach (Archer* archer, _archerList) {
        if (archer->position()==0) continue; // ignore position value 0
        if (checkBucket.contains(archer->position())) _overlapBucket.insert(archer->position());
        checkBucket.insert(archer->position());
    }
}

void ArcherTableModel::dataUpdated(int archerId) {
    int row = 0;
    foreach (Archer* archer, _archerList) {
        if (archer->id()==archerId) {
            emit dataChanged(createIndex(row,0), createIndex(row, ARCHERTABLE_COLUMN_COUNT));
        }
        row++;
    }
}
