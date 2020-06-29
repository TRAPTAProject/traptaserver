#-------------------------------------------------
#
# Project created by QtCreator 2013-08-14T08:34:11
#
#-------------------------------------------------

TARGET = TRAPTA
TEMPLATE = app

QT += network qml quick sql
CONFIG += c++11

unix:!macx {
    QMAKE_LFLAGS += -no-pie
}

SOURCES += main.cpp\
    spreadsheet/spreadsheet.cpp \
    util.cpp \
    database/database.cpp \
    archers/archer.cpp \
    archers/heatscorecard.cpp \
    archers/match.cpp \
    archers/matchscorecard.cpp \
    archers/matchvolleylist.cpp \
    sortingalgo.cpp \
    archers/volley.cpp \
    teams/team.cpp \
    teams/teamcomposermodel.cpp \
    teams/teamrankingmodel.cpp \
    teams/teamrule.cpp \
    teams/teamscore.cpp \
    network/connectivity.cpp \
    network/udpbroadcaster.cpp \
    tablet/tablethandler.cpp \
    tablet/tablethandlerpool.cpp \
    tablet/tabletinfo.cpp \
    tablet/tabletserver.cpp \
    tablet/tabletstatus.cpp \
    datamodel.cpp \
    archers/archerlistmodel.cpp \
    archertablemodel.cpp \
    matchtablemodel.cpp


HEADERS  += \
    spreadsheet/spreadsheet.h \
    define.h \
    util.h \
    define.h \
    database/database.h \
    archers/archer.h \
    archers/heatscorecard.h \
    archers/match.h \
    archers/matchscorecard.h \
    archers/matchvolleylist.h \
    sortingalgo.h \
    archers/volley.h \
    teams/team.h \
    teams/teamcomposermodel.h \
    teams/teamrankingmodel.h \
    teams/teamrule.h \
    teams/teamscore.h \
    network/connectivity.h \
    network/udpbroadcaster.h \
    tablet/tablethandler.h \
    tablet/tablethandlerpool.h \
    tablet/tabletinfo.h \
    tablet/tabletserver.h \
    tablet/tabletstatus.h \
    datamodel.h \
    archers/archerlistmodel.h \
    archertablemodel.h \
    matchtablemodel.h


RESOURCES += \
    resources.qrc

RC_FILE = trapta.rc
ICON = trapta.icns



