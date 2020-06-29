#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QSet>
#include <QList>

#define FILE_CATEGORIES "categories.ini"
#define FILE_CUT "cut.ini"
#define FILE_MSG "log.txt"

class Util {
public:

    static void init();
    static bool resetDB();

    static QString workingDir;
    static QSet<int> invertedRankSet;
    static QList< QList<int> > matchTargetMap;
    static QString categFilePath;
    static QString cutFilePath;
    static QString dbFilePath;
    static QString msgLogFilePath;

    static QString targetLabelFromPosition(int position);
    static int positionFromTargetLabel(const QString& label);
//    static void populateCombobox(QComboBox* combo, int max);
    static int targetIdFromPosition(int position);
    static char letterFromPosition(int position);
    static void debugQByteArray(const QByteArray& array);
    static int getInitRank(int archerIndex, int round, int rank);
    static QString normalize(const QString& string);



};

#endif // UTIL_H
