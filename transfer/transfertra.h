#ifndef TRANSFERTRA_H
#define TRANSFERTRA_H

#include <QObject>
#include <QProcess>

class TransfertRA : public QObject {
    Q_OBJECT
public:
    explicit TransfertRA(QObject *parent = 0);
    qint64 run(int mode); // return pid


signals:

    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void error(QProcess::ProcessError);

private:

    QProcess _process;
    qint64 _pid;
    
};

#endif // TRANSFERTRA_H
