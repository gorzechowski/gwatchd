QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = synchronizeTest

SOURCES += \
    main.cpp \
    rsynccommandpartbasetest.cpp \
    rsynccommandpartexcludestest.cpp \
    rsynccommandpartincludestest.cpp \
    rsynccommandpartremoteshelltest.cpp \
    rsynccommandparttargettest.cpp

HEADERS += \
    rsynccommandpartbasetest.h \
    rsynccommandpartexcludestest.h \
    rsynccommandpartincludestest.h \
    rsynccommandpartremoteshelltest.h \
    rsynccommandparttargettest.h

include(../../../jobs/synchronize/command/command.pri)
include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)

INCLUDEPATH += \
    ../../../core/ \
    ../../../jobs/synchronize/ \
    $$PWD/../../../libs/yaml-cpp/include

QMAKE_RPATHDIR += /usr/lib/gwatchd/

LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp
