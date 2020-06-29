#include "sortingalgo.h"
#include "archers/archer.h"
#include "archers/match.h"
#include "tablet/tabletinfo.h"

int SortingAlgo::_staticRankingHeatIndex = 0;
int SortingAlgo::_staticRankingVolleyIndex = 0;

bool SortingAlgo::alphaRankingLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->name()<archer1->name());

}

bool SortingAlgo::tabletInfoLessThan(TabletInfo* info0, TabletInfo* info1) {
    return (info0->targ()<info1->targ());
}

bool SortingAlgo::positionRankingLessThan(Archer* archer0, Archer* archer1) {
    return (archer0->position()<archer1->position());

}

bool SortingAlgo::shootIdPositionRankingLessThan(Archer* archer0, Archer* archer1) {
    if (archer0->shootId()==archer1->shootId())
        return (archer0->position()<archer1->position());
    return (archer0->shootId()<archer1->shootId());

}

bool SortingAlgo::matchTargetLessThan(Match* match0, Match* match1) {
    return (match0->targetId()<match1->targetId());
}

bool SortingAlgo::matchRankingLessThan(Match* match0, Match* match1) {
    return (match0->rank()<match1->rank());
}


bool SortingAlgo::scoreRankingLessThan(Archer* archer0, Archer* archer1) {
    int score0 = archer0->score(_staticRankingHeatIndex, _staticRankingVolleyIndex);
    int score1 = archer1->score(_staticRankingHeatIndex, _staticRankingVolleyIndex);
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

bool SortingAlgo::scoreFullRankingLessThan(Archer* archer0, Archer* archer1) {
    int score0 = archer0->score();
    int score1 = archer1->score();
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
