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
    rsynccommandparttargettest.cpp \
    ../../../core/config/yamlconfig.cpp \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartbase.cpp \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartexcludes.cpp \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartincludes.cpp \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartremoteshell.cpp \
    ../../../jobs/synchronize/command/rsync/rsynccommandparttarget.cpp \
    ../../../jobs/synchronize/command/rsync/rsynccommandbuilder.cpp

HEADERS += \
    rsynccommandpartbasetest.h \
    rsynccommandpartexcludestest.h \
    rsynccommandpartincludestest.h \
    rsynccommandpartremoteshelltest.h \
    rsynccommandparttargettest.h \
    ../../../core/config/yamlconfig.h \
    ../../../core/config/config.h \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartbase.h \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartexcludes.h \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartincludes.h \
    ../../../jobs/synchronize/command/rsync/rsynccommandpartremoteshell.h \
    ../../../jobs/synchronize/command/rsync/rsynccommandparttarget.h \
    ../../../jobs/synchronize/command/rsync/rsynccommandbuilder.h

INCLUDEPATH += \
    ../../../core/ \
    ../../../jobs/synchronize/ \
    $$PWD/../../../libs/yaml-cpp/include

QMAKE_RPATHDIR += /usr/lib/gwatchd/

LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    config.yml
