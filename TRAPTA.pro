#-------------------------------------------------
#
# Project created by QtCreator 2013-08-14T08:34:11
#
#-------------------------------------------------

QT       += core gui network widgets sql

TARGET = TRAPTA
TEMPLATE = app

unix:!macx {
    QMAKE_LFLAGS += -no-pie
}

SOURCES += \
    archers/archer.cpp \
    archers/archertablemodel.cpp \
    archers/heatscorecard.cpp \
    archers/match.cpp \
    archers/matcharcheritemdelegate.cpp \
    archers/matchmodeitemdelegate.cpp \
    archers/matchscorecard.cpp \
    archers/matchtablemodel.cpp \
    archers/matchtargetitemdelegate.cpp \
    archers/matchvolleylist.cpp \
    archers/volley.cpp \
    controllers/archertablecontroller.cpp \
    controllers/matchtablecontroller.cpp \
    controllers/tablecontroller.cpp \
    database/database.cpp \
    database/datamodel.cpp \
    dialogs/dialogbox.cpp \
    dialogs/dialogmatchname.cpp \
    dialogs/matchsetdialog.cpp \
    dialogs/matchvolleydialog.cpp \
    dialogs/msgbox.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/stringlistdialog.cpp \
    dialogs/textfileeditor.cpp \
    dialogs/volleydialog.cpp \
    main.cpp \
    network/connectivity.cpp \
    network/displayhandler.cpp \
    network/displayserver.cpp \
    network/ipchooser.cpp \
    network/posterhandler.cpp \
    network/posterserver.cpp \
    network/tablethandler.cpp \
    network/tablethandlerpool.cpp \
    network/tabletserver.cpp \
    network/udpbroadcaster.cpp \
    pdf/pdfcreator.cpp \
    pdf/pdfdialog.cpp \
    tablets/tabletinfo.cpp \
    tablets/tabletstatus.cpp \
    teams/team.cpp \
    teams/teamcomposermodel.cpp \
    teams/teamrankingmodel.cpp \
    teams/teamrule.cpp \
    teams/teamscore.cpp \
    transfer/transfertra.cpp \
    trapta.cpp \
    utils/foldercompressor.cpp \
    utils/sortingalgo.cpp \
    utils/util.cpp \
    view/checkboxitemdelegate.cpp \
    view/comboboxitemdelegate.cpp \
    view/lcdspinner.cpp \
    view/lineedititemdelegate.cpp \
    view/waitingspinner.cpp

win32 {
SOURCES += \
    ../traptatransfer/wdrequest.cpp \
    ../traptatransfer/ratransfert.cpp \
    ../traptatransfer/mystandarditemmodel.cpp \
    ../traptatransfer/sqlscript.cpp
}

HEADERS  += \
    archers/archer.h \
    archers/archertablemodel.h \
    archers/heatscorecard.h \
    archers/match.h \
    archers/matcharcheritemdelegate.h \
    archers/matchmodeitemdelegate.h \
    archers/matchscorecard.h \
    archers/matchtablemodel.h \
    archers/matchtargetitemdelegate.h \
    archers/matchvolleylist.h \
    archers/volley.h \
    controllers/archertablecontroller.h \
    controllers/matchtablecontroller.h \
    controllers/tablecontroller.h \
    database/database.h \
    database/datamodel.h \
    define.h \
    dialogs/dialogbox.h \
    dialogs/dialogmatchname.h \
    dialogs/matchsetdialog.h \
    dialogs/matchvolleydialog.h \
    dialogs/msgbox.h \
    dialogs/settingsdialog.h \
    dialogs/stringlistdialog.h \
    dialogs/textfileeditor.h \
    dialogs/volleydialog.h \
    dpointer.h \
    network/connectivity.h \
    network/displayhandler.h \
    network/displayserver.h \
    network/ipchooser.h \
    network/posterhandler.h \
    network/posterserver.h \
    network/tablethandler.h \
    network/tablethandlerpool.h \
    network/tabletserver.h \
    network/udpbroadcaster.h \
    pdf/pdfcreator.h \
    pdf/pdfdialog.h \
    tablets/tabletinfo.h \
    tablets/tabletstatus.h \
    teams/team.h \
    teams/teamcomposermodel.h \
    teams/teamrankingmodel.h \
    teams/teamrule.h \
    teams/teamscore.h \
    transfer/transfertra.h \
    trapta.h \
    utils/foldercompressor.h \
    utils/sortingalgo.h \
    utils/util.h \
    view/checkboxitemdelegate.h \
    view/comboboxitemdelegate.h \
    view/lcdspinner.h \
    view/lineedititemdelegate.h \
    view/waitingspinner.h

win32 {
HEADERS  += \
    ../traptatransfer/wdrequest.h \
    ../traptatransfer/ratransfert.h \
    ../traptatransfer/mystandarditemmodel.h \
    ../traptatransfer/sqlscript.h
}

FORMS    += mainwindow.ui \
    ipchooser.ui \
    settingsdialog.ui \
    volleydialog.ui \
    stringlistdialog.ui \
    dialogmatchname.ui \
    matchvolleydialog.ui \
    pdfdialog.ui \
    matchsetdialog.ui \
    dialogbox.ui \
    textfileeditor.ui
win32 {
FORMS    += \
    ../traptatransfer/ratransfert.ui
}

RESOURCES += \
    resources.qrc \
    breeze.qrc

RC_FILE = trapta.rc
ICON = trapta.icns

OTHER_FILES += \
    cut.txt


