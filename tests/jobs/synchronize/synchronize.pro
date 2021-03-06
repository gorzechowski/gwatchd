include(../../common.pri)

TARGET = synchronizeTest

SOURCES += \
    main.cpp \
    rsynccommandpartbasetest.cpp \
    rsynccommandpartexcludestest.cpp \
    rsynccommandpartincludestest.cpp \
    rsynccommandpartremoteshelltest.cpp \
    rsynccommandparttargettest.cpp \
    rsyncsettingstest.cpp

HEADERS += \
    rsynccommandpartbasetest.h \
    rsynccommandpartexcludestest.h \
    rsynccommandpartincludestest.h \
    rsynccommandpartremoteshelltest.h \
    rsynccommandparttargettest.h \
    rsyncsettingstest.h

include(../../../jobs/synchronize/command/command.pri)
include(../../../jobs/synchronize/config/config.pri)
include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)
include(../../../share/share.pri)

INCLUDEPATH += \
    ../../../jobs/synchronize/
