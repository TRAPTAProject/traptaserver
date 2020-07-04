#ifndef ARCHERTABLECONTROLLER_H
#define ARCHERTABLECONTROLLER_H

#include <QObject>
#include <QTableView>
#include "archertablemodel.h"
#include "database/datamodel.h"
#include <QComboBox>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include "comboboxitemdelegate.h"
#include "lineedititemdelegate.h"
#include "checkboxitemdelegate.h"
#include "tablecontroller.h"

class ArcherTableController : public TableController
{
    Q_OBJECT
public:

    explicit ArcherTableController(
            const QString &name,
            int columnCount,
            QTableView* tableView,
            DataModel* model,
            QMenu *contextMenu,
            QObject *parent = 0);

    void setShootId(int shootId);
    void reset(); // delete all
    void deleteArcherByShootId(int shootId);
    void clearPointsByShootId(int shootId);

    void addNewArcher();

public slots:
    void setRoundCount(int roundCount);


protected slots:
    void processDoubleClick(int row, int column);
    void processCustomMenuDisplay(int row);
    void processDataModelChanged(int archerId);
    void menuSelection(QAction* action);


private:

    ArcherTableModel* _archerTableModel;
    int _currentShootId;
    ComboboxItemDelegate _comboDelegate;
    LineEditItemDelegate _lineEditDelegate;

};

#endif // ARCHERTABLECONTROLLER_H
