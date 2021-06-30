#include "util.h"
#include <QDebug>
#include <qmath.h>
#include <QDir>
#include <QStandardPaths>

QList< QSet<int> > Util::invertedRankSet;
QList< QList<int> > Util::matchTargetMap;
QString Util::workingDir;
QString Util::categFilePath;
QString Util::cutFilePath;
QString Util::dbFilePath;
QString Util::msgLogFilePath;

void Util::init() {
    invertedRankSet.append(QSet<int>());
    invertedRankSet.append(QSet<int>() << 3 << 2);
    invertedRankSet.append(QSet<int>() << 5 << 4 << 7 << 2);
    invertedRankSet.append(QSet<int>() << 9 << 8 << 13 << 4 << 11 << 6 << 15 << 2);

    matchTargetMap.append(QList<int>() << 1 << 2); // big final
    matchTargetMap.append(QList<int>() << 1 << 2); // little final
    matchTargetMap.append(QList<int>() << 1 << 4 << 3 << 2); // semi final (2th)
    matchTargetMap.append(QList<int>() << 1 << 8 << 5 << 4 << 3 << 6 << 7 << 2); // 4th
    matchTargetMap.append(QList<int>() << 1 << 16 << 9 << 8 << 5 << 12 << 13 << 4 << 3 << 14 << 11 << 6 << 7 << 10 << 15 << 2); // 8th

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        qDebug() << "Creating " << dir.absolutePath();
        bool ok = QDir().mkpath(dir.absolutePath());
        if (!ok) qDebug() << "Cannot create " << dir.absolutePath();
    }
    qDebug() << "Using " << dir.absolutePath() << " as data dir";
    workingDir = dir.absolutePath();
    categFilePath = workingDir+"/"+FILE_CATEGORIES;
    cutFilePath = workingDir+"/"+FILE_CUT;
    msgLogFilePath = workingDir+"/"+FILE_MSG;
    dbFilePath = workingDir+"/traptadb.db";

    // check display category file
    if (!QFile::exists(categFilePath)) {
        QFile::copy(":/categories.txt", categFilePath);
        QFile::setPermissions(categFilePath,
                    QFileDevice::ReadOwner|
                    QFileDevice::WriteOwner|
                    QFileDevice::ReadUser|
                    QFileDevice::WriteUser|
                    QFileDevice::ReadGroup|
                    QFileDevice::WriteGroup|
                    QFileDevice::ReadOther|
                    QFileDevice::WriteOther
                              );
    }
    if (!QFile::exists(cutFilePath)) {
        QFile::copy(":/cut.txt", cutFilePath);
        QFile::setPermissions(cutFilePath,
                    QFileDevice::ReadOwner|
                    QFileDevice::WriteOwner|
                    QFileDevice::ReadUser|
                    QFileDevice::WriteUser|
                    QFileDevice::ReadGroup|
                    QFileDevice::WriteGroup|
                    QFileDevice::ReadOther|
                    QFileDevice::WriteOther
                              );
    }


}

bool Util::resetDB() {

    qDebug() << "Removing db file...";
    QFile::remove(dbFilePath);
    qDebug() << "Copying db file...";
    if (!QFile::copy(":/traptadb.db", dbFilePath)) return false;
    qDebug() << "Setting permissions...";
    if (!QFile::setPermissions(dbFilePath,
                    QFileDevice::ReadOwner|
                    QFileDevice::WriteOwner|
                    QFileDevice::ReadUser|
                    QFileDevice::WriteUser|
                    QFileDevice::ReadGroup|
                    QFileDevice::WriteGroup|
                    QFileDevice::ReadOther|
                    QFileDevice::WriteOther
        )) return false;

    return true;
}

// return the label associated with the position: 0=undefined, 1=1A, 2=1B, 3=1C, 4=1D, 5=2A, ...
QString Util::targetLabelFromPosition(int position) {

    if (position==0) return "---";
    int targetIndex = (position-1) / 4;
    int letter = (position-1)%4;

    return QString("%0%1").arg(targetIndex+1).arg(QChar('A'+letter));
}

QString Util::normalize(const QString& string) {
    return string.normalized (QString::NormalizationForm_KD).remove(QRegularExpression("[^a-zA-Z0-9\\s\\-]"));
}

int Util::targetIdFromPosition(int position) {
    int targetIndex = (position-1) / 4;
    if (targetIndex<0) return 0;
    return targetIndex+1;
}

char Util::letterFromPosition(int position) {
    int index = (position-1) % 4;
    return 'A'+index;
}

// return the position associated with the label
int Util::positionFromTargetLabel(const QString& label) {
    if (label.length()<2) return 0;
    // get the last char of the string
    QChar letter = label.at(label.length()-1);
    int modulo = letter.toLatin1()-'A';
    if ((modulo<0) || (modulo>3)) return 0;
    bool ok;
    int target = label.left(label.length()-1).trimmed().toInt(&ok);
    if (!ok) return 0;
    return (target-1)*4+modulo+1;

}

/*
 * returns the rank for archerIndex (0 or 1 in the match), for round and rank
 */
int Util::getInitRank(int archerIndex, int round, int rank) {
    if (rank<1) rank=1;
    if (round<1) round=1;
    if (round==1 && rank==1) return archerIndex+1; // 1 vs 2
    if (round==1 && rank==2) return 4-archerIndex; // 4 vs 3
    if (round<2) return 0;

    int rankA = rank;
    int rankB = (int)(qPow(2, round))+1-rank;

    if (invertedRankSet.value(round-1).contains(rank)) {
        if (archerIndex==0) return rankB;
        else return rankA;
    }
    else {
        if (archerIndex==0) return rankA;
        else return rankB;

    }

}

void Util::populateCombobox(QComboBox* combo, int max) {
    if (!combo) return;
    combo->clear();
    for (int i=1; i<=max; i++) combo->addItem(QString("   %0   ").arg(i));

}

void Util::debugQByteArray(const QByteArray& array) {

    QString string;
    for (int i=0; i<array.size(); i++) {
        if ((array[i]>32) && (array[i]<127))
            string += QString("0x%1").arg((quint8)array[i],2,16,QChar('0')) + "(" + (char)array[i] + ") ";
        else string+= QString("0x%1").arg((quint8)array[i],2,16,QChar('0')) + " ";

    }
    qDebug() << string;

}
