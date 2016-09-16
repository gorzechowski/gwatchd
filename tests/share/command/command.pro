include(../../common.pri)

TARGET = commandTest

SOURCES += \
    main.cpp \
    sshcommandpartbasetest.cpp \
    sshcommandparttargettest.cpp

HEADERS += \
    sshcommandpartbasetest.h \
    sshcommandparttargettest.h

include(../../mockups/mockups.pri)
include(../../../share/share.pri)
include(../../../core/config/config.pri)
