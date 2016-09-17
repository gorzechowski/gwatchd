include(../../common.pri)

TARGET = commandTest

SOURCES += \
    main.cpp \
    commandsettingstest.cpp

HEADERS += \
    commandsettingstest.h

include(../../../jobs/command/config/settings/settings.pri)
include(../../mockups/mockups.pri)
include(../../../share/share.pri)

INCLUDEPATH += \
    ../../../jobs/command/
