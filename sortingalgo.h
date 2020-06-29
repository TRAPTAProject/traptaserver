#ifndef SORTINGALGO_H
#define SORTINGALGO_H


class Archer;
class Match;
class TabletInfo;

class SortingAlgo {

public:
    static bool scoreRankingLessThan(Archer* archer0, Archer* archer1); // this is based on current display heatIndex, VolleyIndex
    static bool scoreFullRankingLessThan(Archer* archer0, Archer* archer1); // this takes full score
    static bool alphaRankingLessThan(Archer* archer0, Archer* archer1);
    static bool positionRankingLessThan(Archer* archer0, Archer* archer1);
    static bool shootIdPositionRankingLessThan(Archer* archer0, Archer* archer1);
    static bool matchTargetLessThan(Match* match0, Match* match1);
    static bool matchRankingLessThan(Match* match0, Match* match1);
    static bool tabletInfoLessThan(TabletInfo* info0, TabletInfo* info1);
    static int _staticRankingHeatIndex;
    static int _staticRankingVolleyIndex;

};

#endif // SORTINGALGO_H
