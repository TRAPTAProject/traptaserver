#ifndef MATCHTABLECONTROLLER_H
#define MATCHTABLECONTROLLER_H

#include <QObject>
#include <QAction>
#include <QTableView>
#include "database/datamodel.h"
#include "tablecontroller.h"
#include "matcharcheritemdelegate.h"
#include "matchtargetitemdelegate.h"
#include "matchmodeitemdelegate.h"

class MatchTableController : public TableController
{
    Q_OBJECT
public:
    explicit MatchTableController(const QString &name,
                                      int columnCount,
                                      QTableView* tableView,
                                      DataModel* model,
                                      QMenu *contextMenu,
                                      QObject *parent = 0);

    void setRoundId(int roundId);
    void reset(); // delete all
    bool deleteMatchRound(int roundId);
    bool deleteMatchByCateg(const QString& categ);
    bool deleteMatchByRow(int row);
    int addRound(const QString& roundName);

public slots:

    void addMatchSet();

protected slots:

    void menuSelection(QAction* action);
    void processMatchModelChanged(int matchId);

protected:

    void processDoubleClick(int row, int column);
    void processCustomMenuDisplay(int row);

private:

    MatchTableModel* _matchTableModel;
    int _currentTabId;
    MatchArcherItemDelegate _archerDelegate;
    MatchTargetItemDelegate _targetDelegate;
    MatchModeItemDelegate _modeDelegate;

};

#endif // MATCHTABLECONTROLLER_H
