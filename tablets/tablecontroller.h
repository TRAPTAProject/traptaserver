#ifndef TABLECONTROLLER_H
#define TABLECONTROLLER_H

#include <QObject>
#include "archertablemodel.h"
#include "datamodel.h"
#include <QTableView>
#include <QMenu>
#include <QAction>

class TableController : public QObject
{
    Q_OBJECT
public:
    explicit TableController(
            const QString &name,
            int columnCount,
            QTableView* tableView,
            DataModel* model,
            QMenu *contextMenu,
            QObject *parent = 0);

    void saveColumnWidth();

    ~TableController();

private slots:
    void onCellDoubleClicked(const QModelIndex& index);
    void customMenuRequested(QPoint pos);

protected slots:

    virtual void menuSelection(QAction* action) = 0;

protected:

    virtual void processDoubleClick(int row, int column) = 0;
    virtual void processCustomMenuDisplay(int row) = 0;

protected:

    int _columnCount;
    QString _name;
    QTableView* _tableView;
    DataModel* _model;
    QMenu* _contextMenu;

    void setColumnWidth();


};

#endif // TABLECONTROLLER_H
