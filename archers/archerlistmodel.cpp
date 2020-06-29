#include "archerlistmodel.h"
#include <QDebug>
#include "datamodel.h"
#include <archers/archer.h>
#include <algorithm>
#include <QSettings>

ArcherListModel::ArcherListModel(DataModel* dataModel) : SpreadSheet(15, QStringList() << "Position"
                                                                                       << "Nom"
                                                                                       << "Catégorie"
                                                                                       << "Club / Équipe"
                                                                                       << "Licence"
                                                                                       << "Trispot"
                                                                                       << "Équipier"
                                                                                       << "Série 1"
                                                                                       << "Série 2"
                                                                                       << "Série 3"
                                                                                       << "Série 4"
                                                                                       << "Total"
                                                                                       << "10"
                                                                                       << "9"
                                                                                       << "Tir",
                                                                                       QList<double>() << 5 << 15 << 6 << 15 << 8 << 5 << 5 << 8 << 8 << 8 << 8 << 5 << 4 << 4 << 3 , 2),
    _dataModel(dataModel),
    _currentShootId(1)
{
    QSettings settings;

    setCheckableForColumn(5);
    setCheckableForColumn(6);

    setComboModelForColumn(0, QStringList() << "01A" << "01B" << "01C" << "01D");
    setActionEnabledForColumn(7);
    setActionEnabledForColumn(8);
    setReadOnlyForColumn(9);
    setReadOnlyForColumn(10);
    setReadOnlyForColumn(11);
    setReadOnlyForColumn(12);
    setTextAlignmentForColumn(0, Qt::AlignHCenter);
    setTextAlignmentForColumn(14, Qt::AlignHCenter);
    setBgColorForColumn(0, "lightgray");

    setSortEnabledForColumn(5, false);
    setSortEnabledForColumn(6, false);
    setSortEnabledForColumn(14, false);

    setCurrentShootId(_currentShootId);
}

ArcherListModel::~ArcherListModel() { }

void ArcherListModel::setCurrentShootId(int shootId) {
    beginResetModel();
    _currentShootId = shootId;
    _archerList = _dataModel->getArcherListSortedByPosition(shootId);
    sortByColumn(0, true);
    endResetModel();
}

void ArcherListModel::requestCheckedChange(int rowIndex, int columnIndex, bool checked) {
    Archer* archer = _archerList.value(rowIndex);
    if (archer==0) {
        qWarning() << "No archer found row " << rowIndex;
        return;
    }
    switch (columnIndex) {
        case 5: {
            archer->setTrispot(!archer->trispot());
            rowUpdated(rowIndex);
            return;
        }
        case 6: {
            archer->setTeammate(!archer->teammate());
            rowUpdated(rowIndex);
            return;
        }
    }

}

void ArcherListModel::requestComboIndexChange(int rowIndex, int columnIndex, int index) {

}

void ArcherListModel::requestTextChange(int rowIndex, int columnIndex, const QString &text) {

}

void ArcherListModel::sortByColumn(int index, bool asc) {
    qDebug() << "Sorting column " << index << " with asc=" << asc;
    beginResetModel();
    switch (index) {
        case 0: {
            if (asc) std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->position()<b->position(); });
            else std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->position()>b->position(); });
            break;
        }
        case 1: {
            if (asc) std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->name()<b->name(); });
            else std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->name()>b->name(); });
            break;
        }
        case 2: {
            if (asc) std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->categ()<b->categ(); });
            else std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->categ()>b->categ(); });
            break;
        }
        case 3: {
            if (asc) std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->noc()<b->noc(); });
            else std::sort(_archerList.begin(), _archerList.end(), [](Archer* a, Archer* b) { return a->noc()>b->noc(); });
            break;
        }
    }

    endResetModel();
}

int ArcherListModel::rowCount() const {
    return _archerList.count();
}

QString ArcherListModel::text(int rowIndex, int columnIndex) const {
    Archer* archer = _archerList.value(rowIndex);
    if (archer!=0) switch (columnIndex) {
        case 1: return archer->name();
        case 2: return archer->categ();
        case 3: return archer->noc();
        case 4: return archer->license();
        case 14: return QString::number(archer->drawId());
    }
    return QString();
}

bool ArcherListModel::checked(int rowIndex, int columnIndex) const {
    Archer* archer = _archerList.value(rowIndex);
    if (archer!=0) switch (columnIndex) {
        case 5: return archer->trispot();
        case 6: return archer->teammate();

    }
    return false;
}

void ArcherListModel::requestAction(int rowIndex, int columnIndex) {
    qDebug() << "Requesting action for cell " << (rowIndex+1) << ":" << (columnIndex+1);
}
