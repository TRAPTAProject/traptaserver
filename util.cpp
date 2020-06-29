#include "util.h"
#include <QDebug>
#include <qmath.h>
#include <QDir>
#include <QStandardPaths>

QSet<int> Util::invertedRankSet;
QList< QList<int> > Util::matchTargetMap;
QString Util::workingDir;
QString Util::categFilePath;
QString Util::cutFilePath;
QString Util::dbFilePath;
QString Util::msgLogFilePath;

void Util::init() {
    invertedRankSet << 1 << 4 << 5 << 8 << 9 << 12 << 13 << 16;
    QList<int> round16;
    round16 << 12 << 5 << 13 << 4 << 9 << 8 << 16 << 1 << 2 << 15 << 7 << 10 << 3 << 14 << 6 << 11;
    QList<int> round8;
    round8 << 5 << 4 << 8 << 1 << 2 << 7 << 3 << 6;
    QList<int> round4;
    round4 << 4 << 1 << 2 << 3;
    QList<int> round2;
    round2 << 1 << 2;
    QList<int> round1;
    round1 << 1 << 2;
    matchTargetMap.append(round1);
    matchTargetMap.append(round2);
    matchTargetMap.append(round4);
    matchTargetMap.append(round8);
    matchTargetMap.append(round16);

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
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
    return string.normalized (QString::NormalizationForm_KD).remove(QRegExp("[^a-zA-Z0-9\\s\\-]"));
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

int Util::getInitRank(int archerIndex, int round, int rank) {
    if (rank<1) rank=1;
    if (round<1) round=1;
    if (round==1 && rank==1) return archerIndex+1; // 1 vs 2
    if (round==1 && rank==2) return 4-archerIndex; // 4 vs 3
    if (round<2) return 0;

    int rankA = rank;
    int rankB = (int)(qPow(2, round))+1-rank;

    if (invertedRankSet.contains(rank)) {
        if (archerIndex==0) return rankB;
        else return rankA;
    }
    else {
        if (archerIndex==0) return rankA;
        else return rankB;

    }

}

//void Util::populateCombobox(QComboBox* combo, int max) {
//    if (!combo) return;
//    combo->clear();
//    for (int i=1; i<=max; i++) combo->addItem(QString("   %0   ").arg(i));

//}

void Util::debugQByteArray(const QByteArray& array) {

    QString string;
    for (int i=0; i<array.size(); i++) {
        if ((array[i]>32) && (array[i]<127))
            string += QString("0x%1").arg((quint8)array[i],2,16,QChar('0')) + "(" + (char)array[i] + ") ";
        else string+= QString("0x%1").arg((quint8)array[i],2,16,QChar('0')) + " ";

    }
    qDebug() << string;

}
