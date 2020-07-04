#include "matchtablecontroller.h"
#include <QDebug>
#include <QHeaderView>
#include "matchsetdialog.h"
#include "util.h"
#include "dialogs/msgbox.h"
#include "sortingalgo.h"
#include "matchvolleydialog.h"

MatchTableController::MatchTableController(
        const QString &name,
        int columnCount,
        QTableView* tableView,
        DataModel* model,
        QMenu *contextMenu,
        QObject* parent) :
    TableController(name, columnCount, tableView, model, contextMenu, parent),
    _matchTableModel(0)
{
    connect(_model, SIGNAL(matchModelChanged(int)), SLOT(processMatchModelChanged(int))); // dataModel changed for archer int

    setRoundId(0);
    _tableView->setItemDelegateForColumn(MatchTableModel::TargetColumn, &_targetDelegate);
    _tableView->setItemDelegateForColumn(MatchTableModel::PlayerAColumn, &_archerDelegate);
    _tableView->setItemDelegateForColumn(MatchTableModel::PlayerBColumn, &_archerDelegate);
    _tableView->setItemDelegateForColumn(MatchTableModel::ModeColumn, &_modeDelegate);

    _tableView->verticalHeader()->hide();
}

int MatchTableController::addRound(const QString& roundName) {
    int roundId = _model->addMatchRound(roundName);
    setRoundId(roundId);
    return roundId;
}

void MatchTableController::setRoundId(int roundId) {
    if (_matchTableModel!=0) saveColumnWidth();
    _matchTableModel = _model->matchTableModel(roundId);
    _tableView->setModel(_matchTableModel);
    _model->setCurrentTabId(roundId);
    _currentTabId = roundId;
    _archerDelegate.setMatchTableModel(_matchTableModel);
    _targetDelegate.setMatchTableModel(_matchTableModel);
    _modeDelegate.setMatchTableModel(_matchTableModel);
    _tableView->sortByColumn(0,Qt::AscendingOrder);

    // this is to refresh table
    setColumnWidth();

}

void MatchTableController::addMatchSet() {
    if (_matchTableModel==0) {
        MsgBox::popup(MsgBox::critical, tr("Créez d'abord un tour de matches"), "", "   OK   ");
        return;
    }

    QMap<int, QString> tabList = _model->getMatchTabMap();
    QString currentTabName = tabList.value(_model->currentTabId());
    tabList.remove(_model->currentTabId());
    QStringList categList = _model->getArcherListByCateg().keys();
    categList.sort();
    MatchSetDialog dialog(currentTabName, categList, tabList.values(), tabList.keys(), _tableView);
    dialog.setModal(true);
    int r = dialog.exec();
    if (r==QDialog::Rejected) return;

    QString categ = dialog.categ();
    int sourceType = dialog.source();
    int previousTabId = dialog.previousMatchTabId();
    int round = dialog.round();
    int target = dialog.target();
    int mode = dialog.mode();
    bool trispot = dialog.trispot();

    // generation based on heat results
    if (sourceType==0) {
        QList<Archer*> archerList = _model->getArcherListSortedByScore(0, categ);
        QList<int> matchMap = Util::matchTargetMap.at(round-1);
        QList<Match*> matchList;
        foreach (int rank, matchMap) {
            int player0 = 0;
            int player1 = 0;
            QString name0 = "";
            QString name1 = "";
            int initRank0 = Util::getInitRank(0, round, rank)-1;
            Archer* archer0 = 0;
            if (initRank0<archerList.size() && initRank0>-1) archer0 = archerList.at(initRank0);
            int initRank1 = Util::getInitRank(1, round, rank)-1;
            Archer* archer1 = 0;
            if (initRank1<archerList.size() && initRank1>-1) archer1 = archerList.at(initRank1);
            if (archer0!=0) {
                player0 = archer0->id();
                name0 = archer0->name();
            }
            if (archer1!=0) {
                player1 = archer1->id();
                name1 = archer1->name();
            }
            Match* match = new Match(0,
                                     _currentTabId,
                                     categ,
                                     round,
                                     rank,
                                     false,
                                     target++,
                                     trispot,
                                     -1,
                                     mode,
                                     player0,
                                     player1);

            match->setArcherName(0, name0);
            match->setArcherName(1, name1);
            matchList << match;
        }
        if (!_model->addMatchList(matchList)) return;
        _matchTableModel->addMatchList(matchList);
        _tableView->sortByColumn(0,Qt::AscendingOrder);
    }
    // generation based on previous match round
    else if (sourceType==1) {

        if (round<1 || round>5) return;
        qDebug() << "Selected tab to create matches: " << previousTabId;
        // get the list of matchModel for this tab
        QList<Match*> matchList = _model->getMatchList(previousTabId, categ);
        qSort(matchList.begin(), matchList.end(), SortingAlgo::matchRankingLessThan);

        if (round==1) {  // final
            Match* match0 = matchList.at(0);
            Match* match1 = matchList.at(1);
            int archer0 = 0;
            int archer1 = 0;
            int archer2 = 0;
            int archer3 = 0;
            if (match0!=0 && match0->winner()>-1) {
                archer0 = match0->archerId(match0->winner());
                archer1 = match0->archerId(match0->looser());
            }

            if (match1!=0 && match1->winner()>-1) {
                archer2 = match1->archerId(match1->winner());
                archer3 = match1->archerId(match1->looser());
            }
            QList<Match*> newMatchList;
            newMatchList << new Match(0, _currentTabId, categ, round, 1, false, target++, trispot, -1, mode, archer0, archer2);
            newMatchList << new Match(0, _currentTabId, categ, round, 2, false, target++, trispot, -1, mode, archer1, archer3);
            if (!_model->addMatchList(newMatchList)) return;
            _matchTableModel->addMatchList(newMatchList);
            _tableView->sortByColumn(0,Qt::AscendingOrder);

        } else {   // semifinal and before
            QList<int> matchMap = Util::matchTargetMap.at(round-1);
            QList<Match*> newMatchList;
            foreach (int rank, matchMap) {

                int initRank0 = Util::getInitRank(0, round, rank)-1;
                Match* match0 = 0;
                if (initRank0<matchList.size() && initRank0>-1) match0 = matchList.at(initRank0);
                int initRank1 = Util::getInitRank(1, round, rank)-1;
                Match* match1 = 0;
                if (initRank1<matchList.size() && initRank1>-1) match1 = matchList.at(initRank1);
                int archer0 = 0;
                int archer1 = 0;
                if (match0!=0 && match0->winner()>-1) archer0 = match0->archerId(match0->winner());
                if (match1!=0 && match1->winner()>-1) archer1 = match1->archerId(match1->winner());

                newMatchList << new Match(0, _currentTabId, categ, round, rank, false, target++, trispot, -1, mode, archer0, archer1);

            }
            if (!_model->addMatchList(newMatchList)) return;
            _matchTableModel->addMatchList(newMatchList);
            _tableView->sortByColumn(0,Qt::AscendingOrder);

        }

    }

}

void MatchTableController::reset() {
    _tableView->setModel(0);
}

bool MatchTableController::deleteMatchRound(int roundId) {
    bool ok = _model->deleteMatchTab(roundId);
    _tableView->setModel(0);
    return ok;
}

bool MatchTableController::deleteMatchByCateg(const QString& categ) {
    _model->deleteMatchCateg(_currentTabId, categ);
    setRoundId(_currentTabId);
    return true;
}

bool MatchTableController::deleteMatchByRow(int row) {
    Match* match = _matchTableModel->matchAt(row);
    _model->deleteMatch(match->id());
    _matchTableModel->removeMatchRow(row);
    return true;
}

void MatchTableController::menuSelection(QAction* action) {

    qDebug() << "Triggered: " << action->text();
    QVariantList paramList = action->data().toList();
    if (paramList.count()<2) return;
    int row = paramList[0].toInt();
    int actionId = paramList[1].toInt();
    Match* match = _matchTableModel->matchAt(row);
    if (actionId==0 && match!=0) {
        int r = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir le match %0:\n%1 vs %2 ?")
                              .arg(match->id()).arg(match->archerName(0)).arg(match->archerName(1)),"   Non   ", "   Oui   ");
        if (r!=QDialog::Accepted) return;
        deleteMatchByRow(row);
    }
    else if (actionId==1) {
        if (paramList.count()<3) return;
        QString categ = paramList.at(2).toString();
        int r = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer la catégorie %0 ?")
                              .arg(categ),"   Non   ", "   Oui   ");
        if (r!=QDialog::Accepted) return;
        deleteMatchByCateg(categ);
    }


}

void MatchTableController::processDoubleClick(int row, int column) {
    qDebug() << "Double click";
    switch (column) {
        case MatchTableModel::TrispotColumn : {
            _matchTableModel->toggleTrispot(row);
            break;
        }
        case MatchTableModel::ScoreAColumn :
        case MatchTableModel::ScoreBColumn : {
            Match* match = _matchTableModel->matchAt(row);
            MatchVolleyDialog dialog(match->archerName(0),
                                     match->scoreCard(0).volleyList(),
                                     match->archerName(1),
                                     match->scoreCard(1).volleyList(),
                                     _tableView);
            dialog.exec();
        }
    }
}

void MatchTableController::processCustomMenuDisplay(int row) {

    Match* match = _matchTableModel->matchAt(row);
    if (match==0) return;
    QVariantList paramList;
    paramList << row; // row
    paramList << 0; // actionId
    _contextMenu->addAction(QString("Effacer match"))->setData(paramList);
    paramList.clear();
    paramList << row; // row
    paramList << 1; // delete categ
    paramList << match->categ();
    _contextMenu->addAction(QString("Effacer catégorie %0").arg(match->categ()))->setData(paramList);


}

void MatchTableController::processMatchModelChanged(int matchId) {
    _matchTableModel->dataUpdated(matchId);
}
