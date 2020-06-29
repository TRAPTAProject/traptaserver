#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "spreadsheet/spreadsheet.h"
#include <QQmlContext>
#include "archers/archerlistmodel.h"
#include "database/database.h"
#include "util.h"
#include "define.h"
#include <QDebug>
#include <QTime>
#include <QSettings>
#include "datamodel.h"

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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("TRAPTA");
    QGuiApplication::setOrganizationDomain("trapta.eu");
    QGuiApplication::setApplicationName("TRAPTAServer");

    qInstallMessageHandler(messageHandler);

    QSettings settings;
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

    ArcherListModel archerListModel(&dataModel);



    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("archerListModel", &archerListModel);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    app.exec();

    return 0;
}
