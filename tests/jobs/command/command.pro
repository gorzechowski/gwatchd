include(../../common.pri)

TARGET = commandTest

SOURCES += \
    main.cpp \
    sshcommandpartbasetest.cpp \
    sshcommandparttargettest.cpp \
    commandconfigtest.cpp

HEADERS += \
    sshcommandpartbasetest.h \
    sshcommandparttargettest.h \
    commandconfigtest.h

include(../../../jobs/command/command/command.pri)
include(../../../jobs/command/config/config.pri)
include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)

INCLUDEPATH += \
    ../../../jobs/command/
