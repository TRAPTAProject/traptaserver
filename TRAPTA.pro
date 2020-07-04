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
    archers/archertablecontroller.cpp \
    archers/archertablemodel.cpp \
    archers/dialogmatchname.cpp \
    archers/heatscorecard.cpp \
    archers/match.cpp \
    archers/matcharcheritemdelegate.cpp \
    archers/matchmodeitemdelegate.cpp \
    archers/matchscorecard.cpp \
    archers/matchsetdialog.cpp \
    archers/matchtablecontroller.cpp \
    archers/matchtablemodel.cpp \
    archers/matchtargetitemdelegate.cpp \
    archers/matchvolleydialog.cpp \
    archers/matchvolleylist.cpp \
    archers/volley.cpp \
    archers/volleydialog.cpp \
    checkboxitemdelegate.cpp \
    comboboxitemdelegate.cpp \
    database/database.cpp \
    database/datamodel.cpp \
    dialogs/dialogbox.cpp \
    dialogs/msgbox.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/stringlistdialog.cpp \
    dialogs/textfileeditor.cpp \
    foldercompressor.cpp \
    lcdspinner.cpp \
    lineedititemdelegate.cpp \
    main.cpp \
    network/connectivity.cpp \
    network/displayhandler.cpp \
    network/displayserver.cpp \
    network/ipchooser.cpp \
    network/posterhandler.cpp \
    network/posterserver.cpp \
    network/udpbroadcaster.cpp \
    pdf/pdfcreator.cpp \
    pdf/pdfdialog.cpp \
    sortingalgo.cpp \
    tablecontroller.cpp \
    tablets/tablethandler.cpp \
    tablets/tablethandlerpool.cpp \
    tablets/tabletinfo.cpp \
    tablets/tabletserver.cpp \
    tablets/tabletstatus.cpp \
    teams/team.cpp \
    teams/teamcomposermodel.cpp \
    teams/teamrankingmodel.cpp \
    teams/teamrule.cpp \
    teams/teamscore.cpp \
    transfertra.cpp \
    trapta.cpp \
    util.cpp \
    waitingspinner.cpp

win32 {
SOURCES += \
    ../traptatransfer/wdrequest.cpp \
    ../traptatransfer/ratransfert.cpp \
    ../traptatransfer/mystandarditemmodel.cpp \
    ../traptatransfer/sqlscript.cpp
}

HEADERS  += \
    archers/archer.h \
    archers/archertablecontroller.h \
    archers/archertablemodel.h \
    archers/dialogmatchname.h \
    archers/heatscorecard.h \
    archers/match.h \
    archers/matcharcheritemdelegate.h \
    archers/matchmodeitemdelegate.h \
    archers/matchscorecard.h \
    archers/matchsetdialog.h \
    archers/matchtablecontroller.h \
    archers/matchtablemodel.h \
    archers/matchtargetitemdelegate.h \
    archers/matchvolleydialog.h \
    archers/matchvolleylist.h \
    archers/volley.h \
    archers/volleydialog.h \
    checkboxitemdelegate.h \
    comboboxitemdelegate.h \
    database/database.h \
    database/datamodel.h \
    define.h \
    dialogs/dialogbox.h \
    dialogs/msgbox.h \
    dialogs/settingsdialog.h \
    dialogs/stringlistdialog.h \
    dialogs/textfileeditor.h \
    dpointer.h \
    foldercompressor.h \
    lcdspinner.h \
    lineedititemdelegate.h \
    network/connectivity.h \
    network/displayhandler.h \
    network/displayserver.h \
    network/ipchooser.h \
    network/posterhandler.h \
    network/posterserver.h \
    network/udpbroadcaster.h \
    pdf/pdfcreator.h \
    pdf/pdfdialog.h \
    sortingalgo.h \
    tablecontroller.h \
    tablets/tablethandler.h \
    tablets/tablethandlerpool.h \
    tablets/tabletinfo.h \
    tablets/tabletserver.h \
    tablets/tabletstatus.h \
    teams/team.h \
    teams/teamcomposermodel.h \
    teams/teamrankingmodel.h \
    teams/teamrule.h \
    teams/teamscore.h \
    transfertra.h \
    trapta.h \
    util.h \
    waitingspinner.h


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


