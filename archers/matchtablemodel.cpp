#include "matchtablemodel.h"
#include <QDebug>
#include "utils/util.h"
#include "utils/sortingalgo.h"
#include "database/datamodel.h"

MatchTableModel::MatchTableModel(int tabId,
                                    const QList<Match *> &matchList,
                                    DataModel* model,
                                    QObject *parent) :

    QAbstractTableModel(parent),
    _matchList(matchList),
    _tabId(tabId),
    _model(model)

{
    checkOverlapTarget();
}

void MatchTableModel::toggleTrispot(int rowIndex) {
    if (rowIndex<0 || rowIndex>=_matchList.count()) return;
    Match* match = _matchList.at(rowIndex);
    match->setTrispot(!match->trispot());
    emit dataChanged(createIndex(rowIndex,TrispotColumn), createIndex(rowIndex, TrispotColumn));
}

QList<Archer *> MatchTableModel::archerComboList(int row, int playerIndex) {

    Match* match = _matchList.at(row);
    QList<Archer*> archerList = *(_model->getArcherListByCateg().value(match->categ()));
    qSort(archerList.begin(), archerList.end(), SortingAlgo::alphaRankingLessThan);
    return archerList;
}

Qt::ItemFlags MatchTableModel::flags(const QModelIndex & index) const {
    int col = index.column();
    if (col==TargetColumn ||
            col==PlayerBColumn ||
            col==PlayerAColumn ||
            col==ModeColumn)
        return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;

}

int MatchTableModel::rowCount(const QModelIndex &parent) const {
    return _matchList.count();
}

int MatchTableModel::columnCount(const QModelIndex &parent) const {
    return MATCHTABLE_COLUMN_COUNT;
}

bool MatchTableModel::addMatchList(QList<Match*> matchList) {
    beginInsertRows(QModelIndex(),_matchList.count(), _matchList.count()+matchList.count()-1);
    _matchList.append(matchList);
    endInsertRows();
    //emit dataChanged(createIndex(0,0), createIndex(_matchList.count(), MATCHTABLE_COLUMN_COUNT));
    return true;
}

bool MatchTableModel::removeMatchRow(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    _matchList.removeAt(row);
    endRemoveRows();
    //emit dataChanged(createIndex(0,0), createIndex(_matchList.count(), MATCHTABLE_COLUMN_COUNT));
    return true;
}


void MatchTableModel::sort(int column, Qt::SortOrder order) {
    qDebug() << "Sorting column " << column << "order " << order;
    if (column==TargetColumn && order==Qt::AscendingOrder) qSort(_matchList.begin(), _matchList.end(), sortTargetLessThan);
    if (column==TargetColumn && order==Qt::DescendingOrder) qSort(_matchList.begin(), _matchList.end(), sortTargetGreaterThan);
    emit dataChanged(createIndex(0,0), createIndex(_matchList.count(), MATCHTABLE_COLUMN_COUNT));
}

bool MatchTableModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (role!=Qt::EditRole) return false;
    int row = index.row();
    int col = index.column();
    if (row>=_matchList.count()) {
        qDebug() << "No match at row " << row;
        return false;
    }
    Match* match = _matchList.at(row);
    switch (col) {
        case TargetColumn: {
            match->setTargetId(value.toInt());
            checkOverlapTarget();
            emit dataChanged(createIndex(0, TargetColumn), createIndex(_matchList.count(), TargetColumn));
            return true;
        }
        case PlayerAColumn:
        case PlayerBColumn: {
            int archerId = value.toInt();
            Archer* archer = _model->getArcherById(archerId);
            if (archer!=0) {
                if (col==PlayerAColumn) {
                    match->setArcherId(0,archerId);
                    match->setArcherName(0, archer->name());
                }
                else {
                    match->setArcherId(1,archerId);
                    match->setArcherName(1, archer->name());
                }
            }

            break;
        }
        case ModeColumn: {
            match->setMode(value.toInt());
        }
        case TrispotColumn: {
            match->setTrispot(value.toBool());
        }

        default: return false;
    }
    emit dataChanged(createIndex(row, col), createIndex(row, col));
    return true;
}

QVariant MatchTableModel::dataForDisplayRole(Match* match, int col) const {

    switch (col) {
        case TargetColumn: {
            if (match->targetId()==0) return QVariant("---");
            return QVariant(match->targetId());
        }
        case CategoryColumn: return QVariant(match->categ());
        case RankAColumn: return QVariant(Util::getInitRank(0, match->round(), match->rank()));
        case PlayerAColumn: return QVariant(match->archerName(0));
        case ScoreAColumn: return QVariant(match->score(0));
        case ScoreBColumn: return QVariant(match->score(1));
        case PlayerBColumn: return QVariant(match->archerName(1));
        case RankBColumn: return QVariant(Util::getInitRank(1, match->round(), match->rank()));
        case ModeColumn: {
            if (match->mode()==1) return QVariant("Points");
            return QVariant("Sets");
        }
        case TrispotColumn: return QVariant();
    }
    return QVariant();
}

QVariant MatchTableModel::dataForTextAlignmentRole(Match* match, int col) const {
    switch (col) {
        case TargetColumn: return QVariant(Qt::AlignCenter);
        case CategoryColumn: return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case RankAColumn: return QVariant(Qt::AlignCenter);
        case PlayerAColumn: return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case ScoreAColumn: return QVariant(Qt::AlignCenter);
        case ScoreBColumn: return QVariant(Qt::AlignCenter);
        case PlayerBColumn: return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case RankBColumn: return QVariant(Qt::AlignCenter);
        case ModeColumn: return QVariant(Qt::AlignCenter);
        case TrispotColumn: return QVariant(Qt::AlignCenter);
    }

    return QVariant();
}

QVariant MatchTableModel::dataForUserRole(Match* match, int col) const {
    switch (col) {
        case TargetColumn: return QVariant(match->targetId());
        case ModeColumn: return QVariant(match->mode());
        case PlayerAColumn: return QVariant(match->archerId(0));
        case PlayerBColumn: return QVariant(match->archerId(1));
    }

    return QVariant();
}

QVariant MatchTableModel::dataForDecorationRole(Match* match, int col) const {
    switch (col) {
        case TrispotColumn: {
            if (match->trispot()) return QVariant(QIcon(":/images/checkbox.png"));
            break;
        }
    }
    return QVariant();
}

QVariant MatchTableModel::dataForBackgroundRole(Match* match, int col) const {
    if (col==TargetColumn && _overlapBucket.contains(match->targetId())) return QVariant(QBrush(QColor(250,0,0)));
    if (col==TargetColumn && match->targetId()==0) return QVariant(QBrush(QColor(250,128,0)));
    if (col==RankAColumn || col==RankBColumn) return QVariant(QBrush(QColor(220,220,220)));
    if (col==ScoreAColumn || col==ScoreBColumn) return QVariant(QBrush(QColor(220,220,220)));

    if (col==PlayerAColumn && match->winner()==0) return QVariant(QBrush(QColor(0,250,0)));
    if (col==PlayerBColumn && match->winner()==1) return QVariant(QBrush(QColor(0,250,0)));

    if (col==PlayerAColumn && match->archerName(0).isEmpty()) return QVariant(QBrush(QColor(250,0,0)));
    if (col==PlayerBColumn && match->archerName(1).isEmpty()) return QVariant(QBrush(QColor(250,0,0)));

    if (col==CategoryColumn) return QVariant(_model->categHue(match->categ()));

    return QVariant();
}


QVariant MatchTableModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int col = index.column();
    if (row>=_matchList.count()) {
        qDebug() << "No match at row " << row;
        return QVariant();
    }
    Match* match = _matchList.at(row);

    switch (role) {
        case Qt::DisplayRole: return dataForDisplayRole(match, col);
        case Qt::UserRole: return dataForUserRole(match, col);
        case Qt::TextAlignmentRole: return dataForTextAlignmentRole(match, col);
        case Qt::DecorationRole: return dataForDecorationRole(match, col);
        case Qt::BackgroundRole: return dataForBackgroundRole(match, col);
    }

    return QVariant();

}

QVariant MatchTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role==Qt::DisplayRole && orientation==Qt::Horizontal) {
        switch (section) {
            case TargetColumn: return QVariant("Cible");
            case CategoryColumn: return QVariant("Catégorie");
            case RankAColumn: return QVariant("Rang A");
            case PlayerAColumn: return QVariant("Nom A");
            case ScoreAColumn: return QVariant("Score A");
            case ScoreBColumn: return QVariant("Score B");
            case PlayerBColumn: return QVariant("Nom B");
            case RankBColumn: return QVariant("Rang B");
            case ModeColumn: return QVariant("Mode");
            case TrispotColumn: return QVariant("Trispot");

        }
    }
    return QVariant();
}

Match* MatchTableModel::matchAt(int index) {
    if (index<0 || index>=_matchList.count()) return 0;
    return _matchList.at(index);
}

bool MatchTableModel::sortTargetLessThan(Match* match0, Match* match1) {
    return (match0->targetId()<match1->targetId());
}

bool MatchTableModel::sortTargetGreaterThan(Match* match0, Match* match1) {
    return (match0->targetId()>match1->targetId());
}

void MatchTableModel::checkOverlapTarget() {
    _overlapBucket.clear();
    QSet<int> checkBucket;
    foreach (Match* match, _matchList) {
        if (match->targetId()==0) continue; // ignore position value 0
        if (checkBucket.contains(match->targetId())) _overlapBucket.insert(match->targetId());
        checkBucket.insert(match->targetId());
    }
}

void MatchTableModel::dataUpdated(int matchId) {
    int row = 0;
    foreach (Match* match, _matchList) {
        if (match->id()==matchId) {
            emit dataChanged(createIndex(row,0), createIndex(row, MATCHTABLE_COLUMN_COUNT));
        }
        row++;
    }
}


MatchTableModel::~MatchTableModel()
{

}

