#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QList>
#include "datamodel.h"
#include "util.h"
#include "volley.h"
#include <QSplashScreen>
#include <QMessageBox>
#include <QDir>
#include "db.h"
#include <QSqlDriver>
#include "define.h"
#include "trapta.h"
#include <QTime>

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray stringDate = QTime::currentTime().toString("hh:mm:ss.zzz").toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "DEBUG|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtWarningMsg:
        fprintf(stdout, "WARNING|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRITICAL|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATAL|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stderr);
        abort();
    }
    fflush(stderr);
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QApplication::setOrganizationName("TRAPTA");
    QApplication::setOrganizationDomain("trapta.eu");
    QApplication::setApplicationName("TRAPTAServer");

    qInstallMessageHandler(messageHandler);

    qDebug() << "Initializing application...";
    QPixmap pixmap(":/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    // check db version and traptadb.db in workingdir
    QSettings settings;
    // if need to copy db file
    Util::init();
    int dbCurrentVersion = settings.value("dbversion", 0).toInt();
    qWarning() << "DB current version: " << dbCurrentVersion;
    if (dbCurrentVersion<DBVERSION || !QFile::exists(Util::dbFilePath)) {
        qWarning() << "Need to upgrade DB. Required version: " << DBVERSION;
        bool ok = Util::resetDB();
        if (ok) {
            settings.setValue("dbversion", DBVERSION);
        }
        else {
            qWarning() << "Cannot upgrade DB";
        }
    }

    qRegisterMetaType< QList<int> >("QList<int>");
    qRegisterMetaType< QList<Volley> >("QList<Volley>");
    qRegisterMetaType< QList< QList<Volley> > >("QList< QList<Volley> >");

    DB sqdb(Util::dbFilePath);

    DataModel dataModel(&sqdb);

    QString styleSheet;
    QFile styleSheetFile(":/stylesheet.qss");
    if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        styleSheet = styleSheetFile.readAll();
    }

    TRAPTA mainWindow(&dataModel);
    mainWindow.setStyleSheet(styleSheet);
    mainWindow.show();
    splash.finish(&mainWindow);
    qDebug() << "Init DONE.";

    return app.exec();
}
