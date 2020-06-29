#include "archertablecontroller.h"
#include <QDebug>
#include <QComboBox>
#include <QSettings>
#include <QMenu>
#include "msgbox.h"
#include "volleydialog.h"
#include <QHeaderView>

#define COLUMN_WIDTH_KEY "ColumnWidth2"

ArcherTableController::ArcherTableController(
        const QString &name,
        int columnCount,
        QTableView* tableView,
        DataModel* model,
        QMenu *contextMenu,
        QObject *parent) :

    TableController(name, columnCount, tableView, model, contextMenu, parent),
    _currentShootId(0)

{

    connect(_model, SIGNAL(dataModelChanged(int)), SLOT(processDataModelChanged(int))); // dataModel changed for archer int


    setShootId(1);
    _tableView->setItemDelegateForColumn(ArcherTableModel::PositionColumn, &_comboDelegate);
    _tableView->setItemDelegateForColumn(ArcherTableModel::LicenseColumn, &_lineEditDelegate);
    _tableView->setItemDelegateForColumn(ArcherTableModel::CategoryColumn, &_lineEditDelegate);
    _tableView->setItemDelegateForColumn(ArcherTableModel::NameColumn, &_lineEditDelegate);
    _tableView->setItemDelegateForColumn(ArcherTableModel::NocColumn, &_lineEditDelegate);

    _tableView->verticalHeader()->hide();

}

void ArcherTableController::setShootId(int shootId) {
    _archerTableModel = _model->archerTableModel(shootId);
    _currentShootId = shootId;
    _tableView->setModel(_archerTableModel);
    _lineEditDelegate.setArcherTableModel(_archerTableModel);
    _comboDelegate.setArcherTableModel(_archerTableModel);
    _tableView->sortByColumn(0,Qt::AscendingOrder);

    // this is to refresh table
    setColumnWidth();
    setRoundCount(_model->roundCount());

}

void ArcherTableController::setRoundCount(int roundCount) {
    for (int i=1; i<=roundCount; i++) {
        _tableView->setColumnHidden(ArcherTableModel::Round1Column+i-1, false);
    }
    for (int i=roundCount+1; i<=ARCHER_MAX_HEAT; i++) {
        _tableView->setColumnHidden(ArcherTableModel::Round1Column+i-1, true);
    }

}

void ArcherTableController::addNewArcher() {
    if (_archerTableModel==0) {
        MsgBox::popup(MsgBox::question, tr("Aucun départ n'est créé.\nAjouter d'abord la liste d'exemple."), "", "   OK   ");
        return;
    }

    Archer* archer = _model->addNewArcher(_currentShootId);
    _archerTableModel->addArcher(archer);

}


void ArcherTableController::processDoubleClick(int row, int column) {

    switch (column) {
        case ArcherTableModel::TrispotColumn : {
            _archerTableModel->toggleTrispot(row);
            break;
        }
        case ArcherTableModel::TeammateColumn : {
            _archerTableModel->toggleTeammate(row);
            break;
        }
        case ArcherTableModel::Round1Column :
        case ArcherTableModel::Round2Column :
        case ArcherTableModel::Round3Column :
        case ArcherTableModel::Round4Column : {
            int roundIndex = column-ArcherTableModel::Round1Column;
            VolleyDialog volleyDialog(_archerTableModel->archerAt(row),
                                      roundIndex,
                                      _model->heatArrowCount(), _tableView);
            volleyDialog.exec();
        }
    }

}

void ArcherTableController::processDataModelChanged(int archerId) {
    _archerTableModel->dataUpdated(archerId);

}

void ArcherTableController::reset() {
    _tableView->setModel(0);
    _model->reset();
    setShootId(1);
}

void ArcherTableController::deleteArcherByShootId(int shootId) {
    // set round 0 (null), then chang model
    _model->deleteArcherByShootId(shootId);
    setShootId(1);
}

void ArcherTableController::clearPointsByShootId(int shootId) {
    _model->clearPointsByShootId(shootId);
}


void ArcherTableController::menuSelection(QAction* action) {

    qDebug() << "Triggered: " << action->text();
    QVariantList list = action->data().toList();
    int row = list[0].toInt();
    //delete archer
    int actionId = list[1].toInt();
    Archer* archer = _archerTableModel->archerAt(row);
    if (archer==0) {
        qDebug() << "No archer at row " << row;
        return;
    }
    if (actionId==0) {

        int r = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer %0 ?").arg(archer->name()), "   Non   ", "   Oui   ");
        if (r!=QDialog::Accepted) return;
        _model->deleteArcherById(archer->id());
        _archerTableModel->removeArcher(row);
    }
    else if (actionId==1) {
        int roundIndex = list[2].toInt();
        int r = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer la série %0 de %1 ?").arg(roundIndex+1).arg(archer->name()), "   Non   ", "   Oui   ");
        if (r!=QDialog::Accepted) return;
        QList<Volley> list;
        _model->processIncomingHeatVolleyList(archer->id(), roundIndex, list);
    }

}

void ArcherTableController::processCustomMenuDisplay(int row) {
    // 1: row index
    // 2: action: 0=delete archer, 1=reset round score
    // 3: data (round index)
    QVariantList list;
    list << row;
    list << 1;
    list << 0;
    for (int i=1; i<=_model->roundCount(); i++) {
        list[2] = i-1;
        _contextMenu->addAction(QString("Remettre à zéro série %0").arg(i))->setData(list);
    }
    list[1] = 0;
    _contextMenu->addAction("Supprimer archer")->setData(list);
}
