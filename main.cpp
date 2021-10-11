#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QList>
#include "database/datamodel.h"
#include "utils/util.h"
#include "archers/volley.h"
#include <QSplashScreen>
#include <QMessageBox>
#include <QDir>
#include "database/database.h"
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

void setSytleSheet(QApplication* app, const QString qss) {
    QFile file(qss);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    app->setStyleSheet(stream.readAll());
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QApplication::setOrganizationName("TRAPTA");
    QApplication::setOrganizationDomain("traptadomain");
    QApplication::setApplicationName("TRAPTAServer");

    qInstallMessageHandler(messageHandler);

    qDebug() << "Initializing application...";
    QPixmap pixmap(":/images/splash.png");
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

    Database sqdb(Util::dbFilePath);

    DataModel dataModel(&sqdb);

    TRAPTA mainWindow(&dataModel);
    if (settings.value("stylesheet", "dark")=="dark") {
        setSytleSheet(&app, ":/dark.qss");
        mainWindow.setColorMenu("Couleurs claires");
    }
    else {
        setSytleSheet(&app, ":/light.qss");
        mainWindow.setColorMenu("Couleurs sombres");
    }

    // Manage style sheet color change and save to settings
    QObject::connect(&mainWindow, &TRAPTA::changeColor,
        [&app, &mainWindow]() {
            QSettings settings;
            if (settings.value("stylesheet", "dark")=="dark") {
                setSytleSheet(&app, ":/light.qss");
                settings.setValue("stylesheet", "light");
                mainWindow.setColorMenu("Couleurs sombres");
            }
            else {
                setSytleSheet(&app, ":/dark.qss");
                settings.setValue("stylesheet", "dark");
                mainWindow.setColorMenu("Couleurs claires");
            }
        }
    );

    mainWindow.show();
    splash.finish(&mainWindow);
    qDebug() << "Init DONE.";

    return app.exec();
}
