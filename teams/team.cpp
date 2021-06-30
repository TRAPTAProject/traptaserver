#include "team.h"
#include "utils/sortingalgo.h"

int Team::_rankingHeatIndex = 0;
int Team::_rankingVolleyIndex = 0;


Team::Team(const QString &name, const QList<Archer *>& archerList, QObject *parent) :
    QObject(parent),
    _archerList(archerList),
    _name(name)
{


}

QList<Archer*> Team::archerList() const {
    return _archerList;
}

QString Team::name() const {
    return _name;
}



TeamScore Team::score(const QString& teamCateg, const QList<TeamRule>& ruleList, int heatIndex, int volleyIndex) const {

    // for each rule in ruleList:
    // get the the list of archers matching this rule.
    // sum up scores
    // create TeamScore
    QList<Archer*> sourceList = _archerList;
    // sort the list
    _rankingHeatIndex = heatIndex;
    _rankingVolleyIndex = volleyIndex;
    std::sort(sourceList.begin(), sourceList.end(), Team::scoreRankingLessThan);
    QList<Archer*> targetList;
    foreach (TeamRule rule, ruleList) {
        // if everybody, take the category and find all archer matching category
        // if not everybody, take the best matching one of the category
        if (rule.everybody()) {
            if (rule.categList().count()<1) continue;  // no categ mentioned
            foreach (Archer* archer, sourceList) {
                if (archer->categ()==rule.categList().at(0)) {
                    targetList.append(archer);
                    sourceList.removeOne(archer);
                }
            }
        }
        else {
            foreach (Archer* archer, sourceList) {
                Archer* foundArcher = 0;
                foreach (QString categ, rule.categList()) {
                    if (archer->categ()==categ) {
                         foundArcher = archer;
                         break;
                    }
                }
                if (foundArcher!=0) {
                    targetList.append(foundArcher);
                    sourceList.removeOne(foundArcher);
                    break;
                }
            }
        }
    }
    // now, I've got all my archers
    int sum = 0;
    foreach (Archer* archer, targetList) {
        int score = archer->score(heatIndex, volleyIndex);
        if (score>0) sum += score;
    }
    return TeamScore(teamCateg, _name, targetList, heatIndex, volleyIndex, sum);

}


bool Team::scoreRankingLessThan(Archer* archer0, Archer* archer1) {
    int score0 = archer0->score(_rankingHeatIndex, _rankingVolleyIndex);
    int score1 = archer1->score(_rankingHeatIndex, _rankingVolleyIndex);
    if (score0==score1) {
        int archer0_crit1 = archer0->criteria1();
        int archer1_crit1 = archer1->criteria1();
        if (archer0_crit1==archer1_crit1) {
            int archer0_crit2 = archer0->criteria2();
            int archer1_crit2 = archer1->criteria2();
            // archer0 has fewer 9 than archer1
            return (archer0_crit2>archer1_crit2);
        }
        // archer0 has fewer 10 than archer1
        return (archer0_crit1>archer1_crit1);
    }
    return (score0>score1);
}

