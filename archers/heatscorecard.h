#ifndef HEATSCORECARD_H
#define HEATSCORECARD_H

#include <QList>
#include "volley.h"
#include <QStandardItemModel>
#include <QJsonArray>

#define HEATSCORECARD_MAX_VOLLEY 12

class HeatScoreCard : public QAbstractTableModel {
    Q_OBJECT
public:

    static bool x10;

    HeatScoreCard();
    HeatScoreCard(const QList<Volley>& volleyList);
    HeatScoreCard(const HeatScoreCard &copy);
    HeatScoreCard& operator = (const HeatScoreCard& copy);

    int score(int volleyIndex) const;
    int score() const;
    int scoreAt(int volleyIndex) const;
    bool strikeAt(int volleyIndex) const;
    int codeCount(int code) const;
    int criteria1() const; // number of 10 or 10+10X
    int criteria2() const;
    Volley volley(int volleyIndex) const;
    QList<Volley> volleyList() const;
    QJsonArray jsonArray() const;
    QJsonArray jsonArray(int volleyIndex) const;
    // returns the number of non empty volley
    int arrowCount() const;  // number of arrows in the longest volley
    int arrowDrawnCount() const; // number of arrow drawn in this heatScoreCard
    int volleyCount() const;

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;


private:

    QList<Volley> _volleyList;
    
};

#endif // HEATSCORECARD_H
