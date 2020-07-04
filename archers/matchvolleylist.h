#ifndef MATCHVOLLEYLIST_H
#define MATCHVOLLEYLIST_H

#include <QStandardItemModel>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include "volley.h"

#define MATCHVOLLEYLIST_MAX_VOLLEY 5

class MatchVolleylist : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit MatchVolleylist(int matchId, int archerIndex, QObject* parent = 0);

    void setVolleyList(const QList<Volley>& list);
    int getScore(int volleyIndex) const;
    int getScore() const;
    int getScoreAt(int volleyIndex) const;
    int getCodeCount(int code) const;
    Volley getVolley(int volleyIndex) const;
    QJsonArray jsonArray() const;
    // returns the number of non empty volley
    int getVolleyCount() const;
    int getArrowCount() const;  // number of arrows in the longest volley
    int getArrowDrawn() const; // number of arrow drawn in this heat

    void setScoreSet(int volleyIndex, int scoreSet);
    int getScoreSet(int volleyIndex) const;
    void setDatabase(QSqlDatabase* db);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

signals:

public slots:

private:


    QList<Volley> _volleyList;
    int _scoreSet[MATCHVOLLEYLIST_MAX_VOLLEY];
    int _matchId;
    int _archerIndex;

    QSqlDatabase* _db;


};

#endif // MATCHVOLLEYLIST_H
