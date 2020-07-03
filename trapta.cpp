#include "trapta.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QDebug>
#include "connectivity.h"
#include "ipchooser.h"
#include "util.h"
#include <QFileDialog>
#include <QSettings>
#include <QLineEdit>
#include <stringlistdialog.h>
#include "dialogmatchname.h"
#include <QKeyEvent>
#include <QBoxLayout>
#include "pdfcreator.h"
#include "pdfdialog.h"
#include <QGraphicsOpacityEffect>
#include "msgbox.h"
#include "textfileeditor.h"
#include "foldercompressor.h"
#include "define.h"
#include "archertablemodel.h"
#include <QtGlobal>


TRAPTA::TRAPTA(DataModel* model) :
    QMainWindow(),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setWindowTitle(QString("  TRAPTA %0").arg(VERSION));
    QSettings settings;

    ui->displayStartCombo->setFocusPolicy(Qt::StrongFocus);
    ui->displayStartCombo->installEventFilter(this);

    _volleyLCDSpinner = new LCDSpinner(1, 10, ui->lcdVolley, ui->nextVolleyButton, ui->prevVolleyButton, this);
    _roundLCDSpinner = new LCDSpinner(1,4,ui->lcdRound, ui->nextRoundButton, ui->previousRoundButton, this);
    connect(_volleyLCDSpinner, SIGNAL(valueChanged(int)), SLOT(displayParamChanged()));
    connect(_roundLCDSpinner, SIGNAL(valueChanged(int)), SLOT(displayParamChanged()));


    _dataModel = model;

    // init match tabs
    foreach (int tabId, _dataModel->getMatchTabMap().keys()) {
        QString name = _dataModel->getMatchTabMap().value(tabId);
        _matchIdIndex.append(tabId);
        QRadioButton* radioButton = new QRadioButton(QString("  %0").arg(name));
        connect(radioButton, SIGNAL(clicked()), SLOT(onMatchTabIndexChanged()));
        ui->matchRoundLayout->layout()->addWidget(radioButton);
        ui->displayRoundBox->addItem(name, QVariant(tabId));
    }
    if (_matchIdIndex.count()>0) {
        QRadioButton* radioButton = (QRadioButton*)ui->matchRoundLayout->layout()->itemAt(0)->widget();
        radioButton->setChecked(true);
    }

    _archerTableController = new ArcherTableController("archerTableController", 15, ui->archerTableView, _dataModel, new QMenu(this), this);
    _matchTableController = new MatchTableController("matchTableController", 10, ui->matchTableView, _dataModel, new QMenu(this), this);
    if (ui->matchRoundLayout->layout()->count()>0) {
        int roundId = _matchIdIndex.at(0);
        _matchTableController->setRoundId(roundId);
    }

    // set roundcount in combobox
    ui->roundCount->setCurrentIndex(_dataModel->roundCount()/2-1);

    // connect roundCount connector to model and table controller
    connect(ui->roundCount, SIGNAL(currentIndexChanged(int)), SLOT(roundCountChanged(int)));
    connect(ui->x10Check, SIGNAL(clicked(bool)), _dataModel, SLOT(set10XForHeat(bool)));

    _warningAddManualArcher = true;
    ui->teamRankingCheckbox->setChecked(_dataModel->publishTeamRanking());
    ui->ignorePreviousStart->setChecked(!_dataModel->ignorePreviousShoot());

    for (int i=1; i<30; i++) ui->displayStartCombo->addItem(QString("Départ %0").arg(i));

    connect(ui->tabletDock, SIGNAL(visibilityChanged(bool)), SLOT(tabletDockVisibilityChanged(bool)));
    connect(ui->displayStartCombo, SIGNAL(currentIndexChanged(int)), SLOT(processShootFilterChanged(int)));
    connect(ui->positionRadioButton, SIGNAL(clicked(bool)), SLOT(processPositionDisplayClicked()));
    connect(ui->serie1Display, SIGNAL(clicked(bool)), SLOT(processDisplaySerie1Clicked()));
    connect(ui->actionSettings, SIGNAL(triggered()), SLOT(processSettingsDialog()));
    connect(ui->matchDisplayButton, SIGNAL(clicked()), SLOT(processMatchDisplayButton()));
    connect(ui->teamRankingCheckbox, SIGNAL(clicked(bool)), _dataModel, SLOT(setPublishTeamRanking(bool)));
    connect(ui->ignorePreviousStart, SIGNAL(clicked(bool)), _dataModel, SLOT(setIgnorePreviousShoot(bool)));
    connect(ui->displayRoundBox, SIGNAL(currentIndexChanged(int)), SLOT(onMatchDisplayTabIndexChanged(int)));

    connect(_dataModel, SIGNAL(inferedVolleyStatusChanged(int,int)), SLOT(onVolleyStatusChanged(int,int)));

    ui->lcdVolley->display(1);
    _dataModel->setCurrentShootId(1);

    ui->tabletTableView->setModel(&_tabletStatus);
    connect(ui->clearStatusButton, SIGNAL(clicked()), &_tabletStatus, SLOT(clear()));

    connect(_dataModel, SIGNAL(tabletMessage(quint32,int,int,QString)), &_tabletStatus, SLOT(updateTabletStatus(quint32,int,int,QString)));
    connect(ui->tabletTableView, SIGNAL(clicked(QModelIndex)), SLOT(tabletTableClicked(QModelIndex)));

    connect(ui->actionImportCSV, SIGNAL(triggered()), SLOT(menuImportCSV()));
    connect(ui->action_Export_PDF_vers_fichier, SIGNAL(triggered()), SLOT(menuPrint()));
    connect(ui->action_Export_PDF_vers_internet, SIGNAL(triggered()), SLOT(menuPrint()));
    connect(ui->actionExportCVS, SIGNAL(triggered()), SLOT(menuExportCSV()));

    connect(ui->action_addArcher, SIGNAL(triggered()),  SLOT(menuAddArcher()));
    connect(ui->action_Version, SIGNAL(triggered()),  SLOT(processAboutClicked()));
    connect(ui->action_Ajouter_tour_de_match, SIGNAL(triggered()), SLOT(menuAddMatchTab()));
    connect(ui->action_delete_points, SIGNAL(triggered()), SLOT(menuClearPoints()));
    connect(ui->action_reset_all_2, SIGNAL(triggered()), SLOT(menuReset()));
    connect(ui->action_delete_current_startId, SIGNAL(triggered()), SLOT(menuDeleteArchers()));
    connect(ui->actionAddSample, SIGNAL(triggered()), SLOT(menuAddSampleList()));
    connect(ui->action_display_categ, SIGNAL(triggered()), SLOT(menuDisplayCateg()));
    connect(ui->action_display_cut, SIGNAL(triggered()), SLOT(menuDisplayCut()));
    connect(ui->action_Enregistrer, SIGNAL(triggered()), SLOT(menuArchive()));
    connect(ui->action_Charger, SIGNAL(triggered()), SLOT(menuRestore()));
    connect(ui->actionSupprimer_tour_de_match_courant, SIGNAL(triggered()), SLOT(menuDeleteMatchTab()));
    connect(ui->action_Ajouter_categorie, SIGNAL(triggered()), _matchTableController, SLOT(addMatchSet()));

    _currentSelectedTablet = -1;
    ui->displayRoundBox->setVisible(false);
    ui->roundWidget->setVisible(false);
    ui->volleyWidget->setVisible(false);

    // Setup match panels
    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), _dataModel, SLOT(setWorkingMode(int)));

    qDebug() << "List of match categories:" << _dataModel->getMatchCategList();

    //*************** link to resultarc
#ifdef Q_OS_WIN
    connect(ui->actionImportResultarc, SIGNAL(triggered()), this, SLOT(menuImportRA()));
    connect(ui->action_Exporter_vers_Resultarc, SIGNAL(triggered()), this, SLOT(menuExportRA()));
#endif

#ifndef Q_OS_WIN
    ui->actionImportResultarc->setVisible(false);
    ui->action_Exporter_vers_Resultarc->setVisible(false);
#endif

    ui->displayStartCombo->setCurrentIndex(0);
    processShootFilterChanged(0);

    ui->runArrowCount->setCurrentIndex(model->heatArrowCount()/3-1);
    ui->runVolleyCount->setValue(model->heatVolleyCount());
    ui->matchArrowCount->setCurrentIndex(model->matchArrowCount()/3-1);
    ui->matchVolleyMax->setValue(model->matchVolleyMax());

    connect(ui->runArrowCount, SIGNAL(currentIndexChanged(int)), SLOT(heatArrowCountChanged(int)));
    connect(ui->matchArrowCount, SIGNAL(currentIndexChanged(int)), SLOT(matchArrowCount(int)));
    connect(ui->runVolleyCount, SIGNAL(valueChanged(int)), _dataModel, SLOT(setHeatVolleyCount(int)));
    connect(ui->matchVolleyMax, SIGNAL(valueChanged(int)), _dataModel, SLOT(setMatchVolleyMax(int)));

    // team tab

    connect(ui->deleteCategButton, SIGNAL(clicked()), SLOT(processDeleteCategButton()));
    connect(ui->addTeamCategButton, SIGNAL(clicked()), _dataModel->teamRankingModel()->composerModel(), SLOT(addRule()));
    ui->categTableView->setModel(_dataModel->teamRankingModel()->composerModel());
    ui->rankingTeamView->setModel(_dataModel->teamRankingModel());
    ui->rankingTeamView->horizontalHeader()->resizeSection(0, 150);
    ui->rankingTeamView->horizontalHeader()->resizeSection(1, 300);
    ui->rankingTeamView->horizontalHeader()->resizeSection(2, 300);

    // msgbox
    MsgBox::init(this);
    _dataModel->setCategoryList(Util::categFilePath);
    _dataModel->setCutList(Util::cutFilePath);

    _greenLightTimer.setSingleShot(true);
    connect(&_greenLightTimer, SIGNAL(timeout()), this, SLOT(greenLightTimeout()));

    QMetaObject::invokeMethod(this, "startServer", Qt::QueuedConnection);
}

void TRAPTA::processDeleteCategButton() {
    int row = ui->categTableView->selectionModel()->currentIndex().row();
    qDebug() << "Removing composer model row " << row;
    _dataModel->teamRankingModel()->composerModel()->removeRule(row);
    _dataModel->rebuildSideMaps();
}

void TRAPTA::heatArrowCountChanged(int value) {
    _dataModel->setHeatArrowCount((value+1)*3);
}

void TRAPTA::matchArrowCount(int value) {
    _dataModel->setMatchArrowCount((value+1)*3);
}

void TRAPTA::onVolleyStatusChanged(int heatIndex, int volleyCount) {
    ui->statusBar->setText(QString("La majorité des archers a envoyé %0 volées pour la série %1").arg(volleyCount).arg(heatIndex+1));
    // if no volley, return
    if (volleyCount<1) return;
    if (heatIndex<0 || heatIndex>ARCHER_MAX_VOLLEY) return;
    // if serie button not checked yet
    ui->serie1Display->setChecked(true);
    ui->roundWidget->setVisible(true);
    ui->volleyWidget->setVisible(true);
    _volleyLCDSpinner->setValue(volleyCount);
    _roundLCDSpinner->setValue(heatIndex+1);
    _dataModel->setDisplayDocumentSelection(DataModel::DisplayDocRanking);
    displayParamChanged();

}

void TRAPTA::processBroadcasting() {
    _greenLightTimer.stop();
    ui->serverGreenLight->setPixmap(QPixmap(":/green-light.png"));
    _greenLightTimer.start(2500);
}

void TRAPTA::greenLightTimeout() {
    ui->serverGreenLight->setPixmap(QPixmap(":/grey-light.png"));
}

void TRAPTA::menuDeleteMatchTab() {
    int currentMatchRoundIndex = getSelectedMatchRoundIndex();
    int currentMatchRoundId = _matchIdIndex.at(currentMatchRoundIndex);
    qDebug() << "Deleting matches for roundId " << currentMatchRoundId;
    int result = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer le tour de matches %0 ?")
                               .arg(_dataModel->getMatchTabMap().value(currentMatchRoundId)), "   Non   ", "   Oui   ");
    if (result==QDialog::Accepted) {
        bool ok = _matchTableController->deleteMatchRound(currentMatchRoundId);
        if (!ok) {
            qDebug() << "cannot delete matches for roundId " << currentMatchRoundId;
            return;
        }
        QRadioButton* radioButton = (QRadioButton*)ui->matchRoundLayout->layout()->itemAt(currentMatchRoundIndex)->widget();
        ui->matchRoundLayout->layout()->removeWidget(radioButton);
        delete radioButton;
        _matchIdIndex.removeAt(currentMatchRoundIndex);
        ui->displayRoundBox->removeItem(currentMatchRoundIndex);
        if (ui->matchRoundLayout->layout()->count()>0) {
            QRadioButton* button = (QRadioButton*)ui->matchRoundLayout->layout()->itemAt(0)->widget();
            button->setChecked(true);
            onMatchTabIndexChanged();
        }
    }
}

void TRAPTA::menuAddMatchTab() {
    DialogMatchName dialog(this);
    int response = dialog.exec();
    if (response==1) {

        QString title = dialog.getRoundName();
        // if title already exists
        if (_dataModel->getMatchTabMap().values().contains(title)) {
            MsgBox::popup(MsgBox::critical, tr("Le tour '%0' existe déjà.").arg(title), "", "     OK     ");
            return;
        }
        int roundId = _matchTableController->addRound(title);
        if (roundId>-1) {
            ui->displayRoundBox->addItem(title, QVariant(roundId));
            _matchIdIndex.append(roundId);
            QRadioButton* radioButton = new QRadioButton(QString("  %0").arg(title));
            connect(radioButton, SIGNAL(clicked()), SLOT(onMatchTabIndexChanged()));
            ui->matchRoundLayout->layout()->addWidget(radioButton);
            radioButton->setChecked(true);

        }

    }
}


void TRAPTA::menuAddArcher() {
    qDebug() << "Adding archer";
    if (_warningAddManualArcher) {
        MsgBox::popup(MsgBox::warning, "Vous ajoutez un archer manuellement sans l'extraire de Result'arc. À la fin des tirs, vous devrez mettre à jour manuellement ses résultats dans Result'arc.", "", "     OK     ");
        _warningAddManualArcher = false;
    }

    _archerTableController->addNewArcher();
}


void TRAPTA::processShootFilterChanged(int index) {
    _archerTableController->setShootId(index+1);
    _dataModel->setCurrentShootId(index+1);
    displayParamChanged();

}


void TRAPTA::tabletTableClicked(QModelIndex modelIndex) {
    int row = modelIndex.row();
    qDebug() << "Tablet table clicked on row " << row;
    /*
    if (_currentSelectedTablet==row) {
        ui->tabletTableView->clearSelection();
        ui->fetchResultButton->setEnabled(false);
        _currentSelectedTablet = -1;
    }
    else {
        _currentSelectedTablet = row;
        ui->fetchResultButton->setEnabled(true);
    }
*/
}

void TRAPTA::processDisplaySerie1Clicked() {
    qDebug() << "Display serie 1 checked: ";
    _dataModel->setDisplayDocumentSelection(DataModel::DisplayDocRanking);
    displayParamChanged();

}

void TRAPTA::processMatchDisplayButton() {
    _dataModel->setDisplayDocumentSelection(DataModel::DisplayDocMatch);
    _dataModel->setCurrentDisplayTabId(ui->displayRoundBox->currentData().toInt());
    _dataModel->updateMatchRanking();
}

void TRAPTA::processPositionDisplayClicked() {
    int startId = ui->displayStartCombo->currentIndex();
    qDebug() << "Displaying position for start " << startId;
    _dataModel->setDisplayDocumentSelection(DataModel::DisplayDocPosition);
    _dataModel->publishPositionList();
}


void TRAPTA::startServer() {
    ui->tabletDock->setVisible(true);
    QStringList ipAddressList = Connectivity::getIPAddressList();
    foreach (QString string, ipAddressList) qDebug() << string;
    if (ipAddressList.size()==0) {
        MsgBox::popup(MsgBox::critical, "Cet ordinateur n'est connecté à aucun réseau. Vérifiez la connexion (WIFI ou câble) avec votre routeur.", "", "     OK     ");
        return;
    }
    _serverAddress = ipAddressList.at(0);
    if (ipAddressList.size()>1) {
        IPChooser ipChooser(ipAddressList, this);
        int r = ipChooser.exec();
        if (r==QDialog::Rejected) return;
        _serverAddress = ipChooser.getSelectedItem();
        if (_serverAddress.isEmpty()) return;
    }

    qDebug() << "Selected network address: " << _serverAddress;

    // get the port for tablet server
    QSettings settings;
    bool manual = settings.value("manualport", false).toBool();
    int port = settings.value("tabletport", 8092).toInt();
    if (!manual) port = 0;

    _tabletThread = new QThread();
    _tabletServer = new TabletServer(port, _dataModel);
    _tabletServer->moveToThread(_tabletThread);
    connect(this, SIGNAL(startTabletServer()), _tabletServer, SLOT(processStartServer()));
    connect(_tabletServer, SIGNAL(tabletServerStarted(bool, int)), this, SLOT(processTabletServerStarted(bool, int)));
    connect(_tabletThread, &QThread::finished, _tabletServer, &QObject::deleteLater);
    _tabletThread->start();
    emit startTabletServer();

    // get the port for display server
    port = settings.value("displayport", 8093).toInt();
    if (!manual) port = 0;

    _displayThread = new QThread();
    _displayServer = new DisplayServer(port, _dataModel);
    _displayServer->moveToThread(_displayThread);
    connect(this, SIGNAL(startDisplayServer()), _displayServer, SLOT(processStartServer()));
    connect(_displayServer, SIGNAL(displayServerStarted(bool,int)), this, SLOT(processDisplayServerStarted(bool,int)));
    connect(_displayThread, &QThread::finished, _displayServer, &QObject::deleteLater);
    _displayThread->start();

}

void TRAPTA::processBroadcastError() {
    qDebug() << "Broadcast error";
    ui->serverInfoLabel->setText(tr("Erreur broadcast: redémarrez l'application"));
    ui->serverGreenLight->setPixmap(QPixmap(":/red-light.png"));
    MsgBox::popup(MsgBox::critical, "Problème réseau. Veuillez redémarrer l'application.", "", "     OK     ");

}

void TRAPTA::processDisplayServerStarted(bool started, int port) {

    _displayPort = port;
    if (!started) {
        MsgBox::popup(MsgBox::critical, "Impossible de démarrer le serveur de synchronisation avec les afficheurs. Vérifiez les droits du pare-feu ou le numéro de port s'il a été fixé manuellement. Il est aussi possible que TRAPTA soit deja lance.", "", "     OK     ");
        return;
    }

    qDebug() << "Display server started: "<< _serverAddress << ":" << _displayPort;
    ui->serverInfoLabel->setText(tr("Serveurs démarrés sur %0:%1,%2").arg(_serverAddress).arg(_tabletPort).arg(_displayPort));

    // UDPBroacaster
#ifdef BROADCASTER_ON
    _broadcaster = new UDPBroadcaster();
    connect(_broadcaster, SIGNAL(broadcastError()), this, SLOT(processBroadcastError()));
    connect(this, SIGNAL(configureBroadcaster(int,int,QString,int,int)), _broadcaster, SLOT(processConfigure(int,int,QString,int,int)));
    connect(_broadcaster, &QThread::finished, _broadcaster, &QObject::deleteLater);
    connect(_broadcaster, SIGNAL(broadcasting()), this, SLOT(processBroadcasting()));
    emit configureBroadcaster(0, 0, _serverAddress, _tabletPort, _displayPort);
#endif

}

void TRAPTA::processTabletServerStarted(bool started, int port) {

    _tabletPort = port;
    if (!started) {
        MsgBox::popup(MsgBox::critical, "Impossible de démarrer le serveur de synchronisation avec les tablettes. Vérifiez les droits du pare-feu ou le numéro de port s'il a été fixé manuellement. Il est aussi possible que TRAPTA soit deja lance.", "", "     OK     ");
        return;
    }

    qDebug() << "Tablet server started: "<< _serverAddress << ":" << port;
    emit startDisplayServer();
}


void TRAPTA::tabletDockVisibilityChanged(bool) {
    ui->tabletDock->setVisible(true);
}


void TRAPTA::processSettingsDialog() {
    qDebug() << "Processing settings dialog";
    SettingsDialog dialog(this);
    dialog.setModal(true);
    int r = dialog.exec();
    if (r==QDialog::Accepted) {
        MsgBox::popup(MsgBox::warning, "L'application doit être redémarrée pour prendre en compte les changements de paramètres.", "", "     OK     ");
        return;
    }
}

void TRAPTA::displayParamChanged() {
    int heatIndex = _roundLCDSpinner->value()-1;
    int volleyIndex = _volleyLCDSpinner->value()-1;
    qDebug() << "heatIndex=" << heatIndex << " volleyIndex=" << volleyIndex;
    _dataModel->setRankingIndex(heatIndex, volleyIndex);

}


TRAPTA::~TRAPTA() {
    delete ui;
}

void TRAPTA::closeEvent(QCloseEvent *event) {
    _archerTableController->saveColumnWidth();
    _matchTableController->saveColumnWidth();
    event->accept();
}

//################################ Key handling
void TRAPTA::keyPressEvent(QKeyEvent* event) {

    switch (event->key()) {

        case Qt::Key_PageUp: {
            _volleyLCDSpinner->inc();
            break;
        }
        case Qt::Key_PageDown: {
            _volleyLCDSpinner->dec();
            break;
        }

    }

    QMainWindow::keyPressEvent(event);
}

//############################### MENU handling


#ifdef Q_OS_WIN

void TRAPTA::menuImportRA() {
    qDebug() << "Import Resultarc";
    RATransfert raTransfer(1);
    raTransfer.setModal(true);
    raTransfer.exec();
    QStringList list = raTransfer.archerList();
    if (list.isEmpty()) return;
    _dataModel->loadStringList(list);
    _archerTableController->setShootId(1);
    ui->displayStartCombo->setCurrentIndex(0);
}

void TRAPTA::menuExportRA() {

    qDebug() << "Export Resultarc";
    int startId = ui->displayStartCombo->currentIndex()+1;
    QStringList stringList;
    QList<Archer*> archerList = _dataModel->getArcherListSortedByName(startId);
    QStringList excludedArcher;
    foreach (Archer* archer, archerList) {
        int archerId = archer->id();
        if (archerId<0) {
            excludedArcher << archer->toString();
            continue;
        }

        int score1 = archer->score(0);
        int score2 = archer->score(1);
        int score3 = archer->score(2);
        int score4 = archer->score(3);
        if (score1+score2+score3+score4==0) continue;
        int nb10 = archer->criteria1();
        int nb9 = archer->criteria2();
        QStringList list;
        list << QString::number(archer->id());
        list << QString::number(nb10);
        list << QString::number(nb9);
        list << QString::number(score1);
        list << QString::number(score2);
        list << QString::number(score3);
        list << QString::number(score4);
        QString string = list.join(';');
        qDebug() << string;
        stringList << string;
    }

    if (!excludedArcher.isEmpty()) {
        StringListDialog stringListDialog("Liste des archers non exportés", "Ces archers ont été ajoutés manuellement et ne pourront donc pas être exportés vers Result'arc. Vous devrez mettre à jour leurs scores manuellement dans Result'arc.", excludedArcher, true, false);
        stringListDialog.exec();

    }
    RATransfert raTransfer(2, stringList, startId);
    raTransfer.setModal(true);
    raTransfer.exec();
}

#endif

void TRAPTA::menuAddSampleList(bool confirm) {
    if (confirm) {
        int r = MsgBox::popup(MsgBox::question, tr("Vous allez ajouter 60 archers comme liste d'exemple dans le départ 1 et 2.\nVoulez-vous continuer ?"), "   Non   ", "   Oui    ");
        if (r==QDialog::Rejected) return;
    }
    menuImportCSV(":/hogwarts.csv");
}

void TRAPTA::menuImportCSV(const QString &tempFilename) {
    QString filename = tempFilename;
    qDebug() << "Import CSV file " << filename;
    if (filename.isEmpty())
        filename = QFileDialog::getOpenFileName(this, tr("Import fichier CSV"),
                                                    "",
                                                    tr("CSV (*.csv);; Tout fichier (*.*)"));
    if (filename.isEmpty()) return;
    QFile file(filename);
    QStringList archerList;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        MsgBox::popup(MsgBox::critical, "Impossible d'ouvrir le fichier "+filename, "", "     OK     ");
        return;
    }
    QTextStream stream(&file);
    QString string;
    while (true) {
        string = stream.readLine().trimmed();
        if (string.isNull()) break;
        if (string.isEmpty()) continue;
        archerList << string;
    }
    _dataModel->loadStringList(archerList);
    _archerTableController->setShootId(1);
    ui->displayStartCombo->setCurrentIndex(0);
}

void TRAPTA::menuDisplayCateg() {
    TextFileEditor editor(":/categories.txt", Util::categFilePath, tr("Ordre d'affichage des catégories et traductions"), tr("Entrez la liste des catégories dans l'ordre d'apparition sur l'afficheur local et sur internet. Pour chaque acronyme, vous pouvez indiquer une transcription plus explicite qui sera utilisée pour l'affichage."), this);
    if (editor.exec()==QDialog::Accepted) {
        _dataModel->setCategoryList(Util::categFilePath);
    }

}

void TRAPTA::menuDisplayCut() {
    TextFileEditor editor(":/cut.txt", Util::cutFilePath, tr("Affichage des cuts par catégorie"), tr("Pour chaque catégorie, vous pouvez préciser combien d'archers seront retenus pour les phases finales. 0 signifie qu'il n'y a pas de phase finale."), this);
    if (editor.exec()==QDialog::Accepted) {
        _dataModel->setCutList(Util::cutFilePath);
    }

}


void TRAPTA::menuPrint() {

    qDebug() << "Printing to PDF";
    QSettings settings;

    PDFDialog dialog(this);
    QString imagePath = settings.value("pdfimagepath", "Nom de ma compétition\nDate et lieu de la compétition").toString();
    QString title = settings.value("pdftitle", "").toString();
    qDebug() << "Title is " << title;
    qDebug() << "Image path is " << imagePath;
    dialog.setImagePath(imagePath);
    dialog.setTitle(title);
    int r = dialog.exec();
    if (r==QDialog::Rejected) return;
    title = dialog.getTitle();
    imagePath = dialog.getImagePath();
    qDebug() << "Title is " << title;
    qDebug() << "Image path is " << imagePath;
    settings.setValue("pdfimagepath", imagePath);
    settings.setValue("pdftitle", title);

    QString filename = settings.value("exportpdf", "trapta.pdf").toString();
    filename = QFileDialog::getSaveFileName(this, tr("Impression dans un PDF"), filename, "Fichier pdf (*.pdf);;Tous les fichiers (*.*)");
    if (filename.isNull() || filename.isEmpty()) return;
    settings.setValue("exportpdf", filename);
    QList<Archer*> archerList = _dataModel->getArcherListSortedByPosition(); // all archers
    PDFCreator pdf;
    bool value = pdf.create(filename, title, imagePath, archerList, _dataModel->roundCount(), _dataModel->heat10X());
    if (value) MsgBox::popup(MsgBox::info, tr("Les feuilles de marque de tous les departs ont été imprimées dans le fichier %0").arg(filename), "", "     OK     ");
    else MsgBox::popup(MsgBox::critical, tr("Impossible d'imprimer dans le fichier %0. Peut-être est-il verrouillé (déjà ouvert) par une autre application ?").arg(filename), "", "     OK     ");

}

void TRAPTA::exportCSV(const QString& filename, int startId) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        MsgBox::popup(MsgBox::critical, "Impossible de créer le fichier "+filename, "", "     OK     ");
        return;
    }
    QTextStream out(&file);
    QList<Archer*> archerList = _dataModel->getArcherListSortedByName(startId);
    QStringList excludedArcher;
    foreach (Archer* archer, archerList) {
        int archerId = archer->id();
        if (archerId<0) {
            excludedArcher << archer->toString();
            continue;
        }

        int score1 = archer->score(0);
        int score2 = archer->score(1);
        int score3 = archer->score(2);
        int score4 = archer->score(3);
        if (score1+score2+score3+score4==0) continue;
        int nb10 = archer->criteria1();
        int nb9 = archer->criteria2();
        QStringList list;
        list << QString::number(archer->id());
        list << QString::number(nb10);
        list << QString::number(nb9);
        list << QString::number(score1);
        list << QString::number(score2);
        list << QString::number(score3);
        list << QString::number(score4);
        QString string = list.join(';');
        qDebug() << string;
        out << string << "\n";
    }
    file.close();
    if (!excludedArcher.isEmpty()) {
        StringListDialog stringListDialog("Liste des archers non exportés", "Ces archers ont été ajoutés manuellement et ne pourront donc pas être exportés vers Result'ARC. Vous devrez mettre à jour leurs scores manuellement dans Result'ARC.", excludedArcher, true, false);
        stringListDialog.exec();

    }
    MsgBox::popup(MsgBox::info, tr("Les résultats du départ %0 ont été exportés dans le fichier %1").arg(startId).arg(filename), "", "     OK     ");

}

void TRAPTA::menuClearPoints() {
    int currentStartId = ui->displayStartCombo->currentIndex()+1;
    int result = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer les points des archers du départ %0 ?").arg(currentStartId), "   Non   ", "   Oui   ");
    if (result==QDialog::Accepted) _archerTableController->clearPointsByShootId(currentStartId);


}

void TRAPTA::processAboutClicked() {
    qDebug() << "About clicked";
    MsgBox::popup(MsgBox::idea, QString("TRAPTA %0\n%1\nwww.trapta.fr\nDatabase: %2").arg(VERSION).arg(DATE).arg(Util::workingDir), "", "     OK     ");
}

void TRAPTA::menuExportCSV() {
    qDebug() << "Processing export CSV";
    int startId = ui->displayStartCombo->currentIndex()+1;
    QSettings settings;
    QString filename = settings.value("exportcsv", "trapta.csv").toString();
    filename = QFileDialog::getSaveFileName(this, tr("Export vers fichier CSV"), filename, "Fichier csv (*.csv);;Tous les fichiers (*.*)");
    if (filename.isNull() || filename.isEmpty()) return;
    settings.setValue("exportcsv", filename);
    exportCSV(filename, startId);
}

bool TRAPTA::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::Wheel) {
        QComboBox* combo = qobject_cast<QComboBox*>(obj);
        if (combo && !combo->hasFocus()) return true;
    }
    return false;
}

void TRAPTA::menuReset() {
    int result = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer toutes les données ?\n(listes d'archers et matches)"), "   Non   ", "   Oui   ");
    if (result==QDialog::Accepted) {
        _archerTableController->reset();
        _matchTableController->reset();
        _dataModel->reset();
        ui->displayRoundBox->clear();
        QLayoutItem* item;
        while ((item = ui->matchRoundLayout->layout()->takeAt(0)) != 0) delete item;
        _matchIdIndex.clear();
    }
}

void TRAPTA::menuDeleteArchers() {
    int currentShootId = ui->displayStartCombo->currentIndex()+1;
    qDebug() << "Deleting archers for roundId " << currentShootId;
    int result = MsgBox::popup(MsgBox::question, tr("Êtes-vous sûrs de vouloir effacer tous les archers du départ %0 ?").arg(currentShootId), "   Non   ", "   Oui   ");
    if (result==QDialog::Accepted) _archerTableController->deleteArcherByShootId(currentShootId);
    ui->displayStartCombo->setCurrentIndex(0);
}

void TRAPTA::menuArchive() {
    FolderCompressor compressor;
    QString filename = "test.tra";
    bool ok = compressor.compressFolder(Util::workingDir, filename);
    if (ok) {
        qDebug() << Util::workingDir << " compressed to " << filename;
    }
    else {
        qDebug() << "Couldn't compress " << Util::workingDir << " to " << filename;
    }
}

int TRAPTA::getSelectedMatchRoundIndex() {
    // find which one in checked
    int index = -1;
    for (int i=0; i<ui->matchRoundLayout->layout()->count(); i++) {
        QRadioButton* radioButton = (QRadioButton*)ui->matchRoundLayout->layout()->itemAt(i)->widget();
        if (radioButton->isChecked()) {
            index = i;
            break;
        }
    }
    return index;
}

void TRAPTA::onMatchTabIndexChanged() {
    int index = getSelectedMatchRoundIndex();
    if (index<0) return;
    _matchTableController->setRoundId(_matchIdIndex.at(index));
}

void TRAPTA::onMatchDisplayTabIndexChanged(int index) {
    _dataModel->setCurrentDisplayTabId(ui->displayRoundBox->currentData().toInt());
}


void TRAPTA::roundCountChanged(int value) {
    int roundCount = 2+value*2;
    _dataModel->setRoundCount(roundCount);
    _archerTableController->setRoundCount(roundCount);

}

void TRAPTA::menuRestore() {
//    _matchController->deleteAllMatchTabs();
//    _dataModel->restore("test.tra");
//    delete _tableController;
//    delete _matchController;
//    _tableController = new TableController(_tableView, _dataModel, this);
//    _matchController = new MatchController(ui->tabWidget, ui->displayRoundBox, _dataModel);
//    processStartFilterChanged(0);
}
