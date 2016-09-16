include(../../common.pri)

TARGET = configTest

SOURCES += \
    main.cpp \
    sshsettingstest.cpp

HEADERS += \
    sshsettingstest.h

include(../../../share/config/settings/settings.pri)
include(../../mockups/mockups.pri)
