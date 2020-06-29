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

SOURCES += main.cpp\
    archer.cpp \
    volleydialog.cpp \
    util.cpp \
    connectivity.cpp \
    ipchooser.cpp \
    trapta.cpp \
    volley.cpp \
    datamodel.cpp \
    udpbroadcaster.cpp \
    settingsdialog.cpp \
    tabletstatus.cpp \
    displayserver.cpp \
    tablethandlerpool.cpp \
    tablethandler.cpp \
    tabletserver.cpp \
    displayhandler.cpp \
    stringlistdialog.cpp \
    dialogmatchname.cpp \
    matchvolleydialog.cpp \
    posterserver.cpp \
    posterhandler.cpp \
    pdfcreator.cpp \
    pdfdialog.cpp \
    teamcomposermodel.cpp \
    match.cpp \
    heatscorecard.cpp \
    matchscorecard.cpp \
    matchsetdialog.cpp \
    team.cpp \
    sortingalgo.cpp \
    teamrankingmodel.cpp \
    tabletinfo.cpp \
    teamrule.cpp \
    teamscore.cpp \
    dialogbox.cpp \
    msgbox.cpp \
    textfileeditor.cpp \
    foldercompressor.cpp \
    archertablemodel.cpp \
    archertablecontroller.cpp \
    db.cpp \
    lcdspinner.cpp \
    comboboxitemdelegate.cpp \
    lineedititemdelegate.cpp \
    matchtablecontroller.cpp \
    matchtablemodel.cpp \
    checkboxitemdelegate.cpp \
    tablecontroller.cpp \
    matchtargetitemdelegate.cpp \
    matcharcheritemdelegate.cpp \
    matchmodeitemdelegate.cpp


win32 {
SOURCES += \
    ../traptatransfer/wdrequest.cpp \
    ../traptatransfer/ratransfert.cpp \
    ../traptatransfer/mystandarditemmodel.cpp \
    ../traptatransfer/sqlscript.cpp
}

HEADERS  += \
    archer.h \
    volleydialog.h \
    util.h \
    connectivity.h \
    ipchooser.h \
    trapta.h \
    volley.h \
    datamodel.h \
    udpbroadcaster.h \
    settingsdialog.h \
    tabletstatus.h \
    displayserver.h \
    tablethandlerpool.h \
    tablethandler.h \
    tabletserver.h \
    displayhandler.h \
    stringlistdialog.h \
    dialogmatchname.h \
    matchvolleydialog.h \
    posterserver.h \
    posterhandler.h \
    pdfcreator.h \
    pdfdialog.h \
    teamcomposermodel.h \
    match.h \
    heatscorecard.h \
    matchscorecard.h \
    matchsetdialog.h \
    team.h \
    sortingalgo.h \
    teamrankingmodel.h \
    tabletinfo.h \
    teamrule.h \
    teamscore.h \
    dialogbox.h \
    msgbox.h \
    textfileeditor.h \
    foldercompressor.h \
    archertablemodel.h \
    archertablecontroller.h \
    db.h \
    define.h \
    lcdspinner.h \
    comboboxitemdelegate.h \
    lineedititemdelegate.h \
    matchtablecontroller.h \
    matchtablemodel.h \
    checkboxitemdelegate.h \
    tablecontroller.h \
    matchtargetitemdelegate.h \
    matcharcheritemdelegate.h \
    matchmodeitemdelegate.h


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
    resources.qrc

RC_FILE = trapta.rc
ICON = trapta.icns

OTHER_FILES += \
    cut.txt


