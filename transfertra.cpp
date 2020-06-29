#include "transfertra.h"
#include <QDebug>
#include <QApplication>

TransfertRA::TransfertRA(QObject *parent) :
    QObject(parent) {

    connect(&_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SIGNAL(processFinished(int,QProcess::ExitStatus)));
    connect(&_process, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    connect(&_process, SIGNAL(error(QProcess::ProcessError)), this, SIGNAL(error(QProcess::ProcessError)));

}



Q_PID TransfertRA::run(int mode) {
    qDebug() << "Starting process TransfertRA";
    QStringList argList;
    argList << QString::number(mode);
    _process.start(QApplication::applicationDirPath()+"/TransfertResultarc.exe", argList);
    _pid = _process.pid();
    return _pid;
}
