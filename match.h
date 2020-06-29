#ifndef MATCH_H
#define MATCH_H

#include <QObject>
#include "archer.h"
#include <QSet>
#include <QJsonDocument>
#include "matchscorecard.h"


class DB;

class Match: public QObject {

    Q_OBJECT

public:

    static int maxVolleyCount;

    enum {
        modeSet = 0,
        modePoint
    };

    explicit Match(int id,
                   int tabId,
                   const QString &categ,
                   int round,
                   int rank,
                   bool manual,
                   int targetId,
                   bool trispot,
                   int tieBreakWinner,
                   int mode,
                   int player0, int player1,
                   DB* db = 0);
    ~Match();

    void setDB(DB* db);
    void setId(int id);
    void setArcherId(int index, int archerId);
    int archerId(int index);
    QString archerName(int index) const;
    void setWinner(int index);
    void setArcherName(int index, const QString& name);

    void setScoreCard(const MatchScoreCard &scoreCard0, const MatchScoreCard& scoreCard1, bool commit=true); // done when coming from tablet
    void checkScoreSet();
    void setMode(int mode);
    void setTrispot(bool value);
    int mode();

    int winner();
    int looser();
    int targetId();
    bool trispot();
    QString categ() const;
    int id();
    int tabId();
    int round();
    int rank();
    int tieBreak(); // -1: no tie break, 0 or 1
    MatchScoreCard scoreCard(int archerIndex);
    int score(int index);

    bool manualRank();

    QJsonObject jsonDataSet() const;
    QJsonObject jsonResultSet() const;
    QJsonObject jsonForDisplay() const;



public slots:

    void setTargetId(int targetId);

private:


    int _id;
    int _tabId;
    int _winner;
    int _tieBreakWinner;
    int _targetId;
    int _scoreSet[2][MATCHSCORECARD_MAX_VOLLEY];
    int _score[2];
    int _latestVolley[2];
    int _round;
    int _rank;
    bool _trispot;
    bool _manualRank;
    int _rankA;
    int _rankB;
    int _mode;   // 0 if using set, 1 if using points
    int _archerId[2];
    QString _archerName[2];
    MatchScoreCard _scorecard[2];
    QString _categ;
    DB* _db;


    QJsonArray markerList() const;
    void updateDB();

};

#endif // MATCH_H
