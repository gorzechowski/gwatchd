include(../../common.pri)

TARGET = commandTest

SOURCES += \
    main.cpp \
    commandconfigtest.cpp

HEADERS += \
    commandconfigtest.h

include(../../../jobs/command/config/config.pri)
include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)
include(../../../share/share.pri)

INCLUDEPATH += \
    ../../../jobs/command/
