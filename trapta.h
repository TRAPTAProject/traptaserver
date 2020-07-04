#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "tabletserver.h"
#include "udpbroadcaster.h"
#include "settingsdialog.h"
#include "tabletstatus.h"
#include "displayserver.h"
#include <QProcess>
#include <QLabel>
#include "archertablemodel.h"
#include "archertablecontroller.h"
#include "matchtablecontroller.h"
#include "lcdspinner.h"

#define BROADCASTER_ON
#define DISABLE_ROLLCALL

#ifdef Q_OS_WIN32
#include "../traptatransfer/ratransfert.h"
#endif


namespace Ui {
class MainWindow;
}

class TRAPTA : public QMainWindow {
    Q_OBJECT
    
public:
    explicit TRAPTA(DataModel* model);
    ~TRAPTA();
    bool eventFilter(QObject *obj, QEvent *e);
    
signals:

    void startTabletServer();
    void startDisplayServer();
    void configureBroadcaster(int command, int heatIndex, QString address, int portTablet, int portDisplay);
    void changeColor();

protected:
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent *event);

public slots:

    void processTabletServerStarted(bool started, int port);
    void processDisplayServerStarted(bool started, int port);
    void tabletDockVisibilityChanged(bool);
    void processBroadcastError();
    void processShootFilterChanged(int index);
    void processDisplaySerie1Clicked();
    void processPositionDisplayClicked();
    void processMatchDisplayButton();
    void processSettingsDialog();
    void tabletTableClicked(QModelIndex modelIndex);
    void displayParamChanged();
    void processAboutClicked();
    void menuAddSampleList(bool confirm = true);
    void onVolleyStatusChanged(int heatIndex, int volleyCount);
    void processBroadcasting();
    void greenLightTimeout();
    void setColorMenu(const QString& label);

private slots:

    void menuImportCSV(const QString& filename = QString());
    void menuPrint();
    void menuExportCSV();
    void menuClearPoints();
    void menuAddArcher();
    void menuReset();
    void menuDeleteArchers();
    void menuAddMatchTab();
    void menuDeleteMatchTab();
    void menuDisplayCateg();
    void menuDisplayCut();
    void menuArchive();
    void menuRestore();
    void menuChangeColor();

    void heatArrowCountChanged(int value);
    void matchArrowCount(int value);
    void processDeleteCategButton();
    void roundCountChanged(int value);
    void onMatchTabIndexChanged();
    void onMatchDisplayTabIndexChanged(int index);

#ifdef Q_OS_WIN32

    void menuImportRA();
    void menuExportRA();

#endif

private slots:
    void startServer();

private:
    Ui::MainWindow *ui;

    int _displayPort;
    int _tabletPort;
    int _transfertMode;
    bool _warningAddManualArcher;

    QList<int> _matchIdIndex;
    DataModel* _dataModel;
    ArcherTableController* _archerTableController;
    MatchTableController* _matchTableController;
    QThread* _tabletThread;
    QThread* _displayThread;
    TabletServer* _tabletServer;
    DisplayServer* _displayServer;
    QString _serverAddress;
    UDPBroadcaster* _broadcaster;
    TabletStatus _tabletStatus;
    int _currentSelectedTablet;
    LCDSpinner* _volleyLCDSpinner;
    LCDSpinner* _roundLCDSpinner;
    QTimer _greenLightTimer;

    void importCSV(const QString& filename);
    void exportCSV(const QString& filename, int startId);
    int getSelectedMatchRoundIndex();

};

#endif // MAINWINDOW_H
