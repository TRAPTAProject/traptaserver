#ifndef MATCHSCORECARD_H
#define MATCHSCORECARD_H

#include <QAbstractTableModel>
#include "volley.h"

#define MATCHSCORECARD_MAX_VOLLEY 5

class MatchScoreCard : public QAbstractTableModel {
    Q_OBJECT
public:

    MatchScoreCard();
    MatchScoreCard(const QList<Volley>& volleyList);
    MatchScoreCard(const MatchScoreCard &copy);
    MatchScoreCard& operator = (const MatchScoreCard& copy);

    int score(int volleyIndex) const;
    int score() const;
    int scoreAt(int volleyIndex) const;
    int codeCount(int code) const;
    Volley volley(int volleyIndex) const;
    QList<Volley> volleyList() const;
    QJsonArray jsonArray() const;
    // returns the number of non empty volley
    int volleyCount() const;
    int arrowCount() const;  // number of arrows in the longest volley
    int arrowDrawn() const; // number of arrow drawn in this heatScoreCard

    void setScoreSet(int volleyIndex, int scoreSet);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

signals:

public slots:

private:


    QList<Volley> _volleyList;
    int _scoreSet[MATCHSCORECARD_MAX_VOLLEY];


};

#endif // MATCHSCORECARD_H
