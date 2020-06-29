#ifndef ARCHER_H
#define ARCHER_H

#include <QList>
#include "heatscorecard.h"
#include <QByteArray>
#include <QJsonObject>
#include "db.h"

#define ARCHER_MAX_HEAT 4
#define ARCHER_MAX_VOLLEY 12
#define ARCHER_MAX_GLOBAL_VOLLEY 48 // ARCHER_MAX_HEAT*ARCHER_MAX_VOLLEY

class DB;

class Archer : public QObject {
    Q_OBJECT
public:

    explicit Archer(int id,
                        const QString& license,
                        const QString& category,
                        const QString& firstname,
                        const QString& lastname,
                        const QString& noc,
                        int shootId,
                        int position,
                        bool trispot,
                        bool teammate,
                        DB* db = 0
            );

    // return the sum of all runs if index = -1
    int score(int heatIndex) const;
    int score(int heatIndex, int volleyIndex) const;
    int score() const; //equivalent to score(-1)
    int codeCount(int value) const;
    int criteria1() const;
    int criteria2() const;
    bool sameAs(Archer* archer);
    QString name() const;
    QString firstName() const;
    QString lastName() const;
    QString license() const;
    QString categ() const;
    QString noc() const;
    int drawId() const;
    int id() const;
    bool trispot() const;
    bool teammate() const;
    bool warned(int heatIndex) const;
    void setWarning(int heatIndex, bool value);
    int shootId() const;
    int position() const;
    float average() const;
    int trend(int heatIndex, int volleyIndex) const;
    int arrowDrawnCount() const;
    void setScoreCard(int heatIndex, const HeatScoreCard& scoreCard, bool commit=true); // done when coming from tablet
    QJsonObject jsonForTablet() const;
    QJsonObject jsonForPosition() const;
    QJsonObject jsonForRanking(int rankingHeatIndex, int rankingVolleyIndex, int globalVolleyIndex, bool includeTrend, bool includePosition) const;
    QJsonArray jsonArrowList() const;
    QJsonArray jsonArrowList(int heatIndex, int volleyIndex) const;
    QString toString() const;
    QString toTitleString() const;
    QList<HeatScoreCard> scoreCardList() const;

    HeatScoreCard scoreCard(int heatIndex) const;
    // returns the number of non empty volley
    int volleyCount() const; // adding all heatScoreCard
    int volleyCount(int heatIndex) const; // for a specific heatScoreCard

    void setLicense(const QString& getLicense);
    void setCategory(const QString& getCateg);
    void setName(const QString& getName);
    void setNoc(const QString& noc);
    void setPosition(int position);
    void setRank(int globalVolleyIndex, int rank);
    void setShootId(int shootId);
    void setDisplayed(bool value);
    void setDrawId(int draw);
    void setTrispot(bool value);
    void setTeammate(bool value);

    // this is done without DB update !
    void resetScoreCard(int heatIndex);

signals:

    void archerEdited(int id);

private:

    int _id;
    QString _license;
    QString _name;
    QString _firstname;
    QString _lastname;
    QString _noc;
    QString _category;
    int _drawId;
    int _shootId;    // starts at 1
    int _position;  // starts at 1 (position 0 is 'not defined')
    QList<HeatScoreCard> _scoreCardList;
    DB* _db;
    bool _trispot;
    bool _teammate;
    bool _warning[ARCHER_MAX_HEAT];
    int _rank[ARCHER_MAX_GLOBAL_VOLLEY];

    void updateDB();

};

#endif // ARCHER_H
