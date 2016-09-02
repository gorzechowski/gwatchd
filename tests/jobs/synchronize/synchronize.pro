include(../../common.pri)

TARGET = synchronizeTest

SOURCES += \
    main.cpp \
    rsynccommandpartbasetest.cpp \
    rsynccommandpartexcludestest.cpp \
    rsynccommandpartincludestest.cpp \
    rsynccommandpartremoteshelltest.cpp \
    rsynccommandparttargettest.cpp \
    synchronizeconfigtest.cpp

HEADERS += \
    rsynccommandpartbasetest.h \
    rsynccommandpartexcludestest.h \
    rsynccommandpartincludestest.h \
    rsynccommandpartremoteshelltest.h \
    rsynccommandparttargettest.h \
    synchronizeconfigtest.h

include(../../../jobs/synchronize/command/command.pri)
include(../../../jobs/synchronize/config/config.pri)
include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)

INCLUDEPATH += \
    ../../../jobs/synchronize/

#QMAKE_RPATHDIR += $$OUT_PWD/../../../libs/yaml-cpp

#LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp
