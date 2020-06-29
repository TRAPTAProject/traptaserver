#include "tablecontroller.h"
#include <QSettings>

TableController::TableController(const QString& name, int columnCount,
        QTableView *tableView,
        DataModel* model,
        QMenu* contextMenu,
        QObject *parent) :

        _name(name),
        _columnCount(columnCount),
        _tableView(tableView),
        _model(model),
        _contextMenu(contextMenu),
        QObject(parent)
{
    connect(_tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(onCellDoubleClicked(QModelIndex)));
    _tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    connect(_contextMenu, SIGNAL(triggered(QAction*)), SLOT(menuSelection(QAction*)));

}

void TableController::onCellDoubleClicked(const QModelIndex& index) {
    processDoubleClick(index.row(), index.column());
}


void TableController::setColumnWidth() {

    QSettings settings;
    QVariant variant = settings.value(_name);
    int col = 0;
    foreach (QVariant value, variant.toList()) {
        bool ok;
        int width = value.toInt(&ok);
        if (!ok) width = 80;
        _tableView->setColumnWidth(col++, width);
    }

}

void TableController::saveColumnWidth() {
    QSettings settings;
    QVariantList list;
    for (int i=0; i<_columnCount; i++) {
        int width = _tableView->columnWidth(i);
        if (width==0) width = 80; // if column hidden, its width is 0, do not take it
        list << QVariant(width);
    }
    settings.setValue(_name, QVariant(list));
}

void TableController::customMenuRequested(QPoint pos) {
    QModelIndex index = _tableView->indexAt(pos);
    // clear menu
    _contextMenu->clear();
    // populate menu
    processCustomMenuDisplay(index.row());
    // display menu
    _contextMenu->popup(_tableView->viewport()->mapToGlobal(pos));
}

TableController::~TableController()
{

}

