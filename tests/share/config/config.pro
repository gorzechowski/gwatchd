include(../../common.pri)

TARGET = configTest

SOURCES += \
    main.cpp \
    sshsettingstest.cpp \
    hookdescriptortest.cpp \
    hookssettingstest.cpp \
    settingstest.cpp

HEADERS += \
    sshsettingstest.h \
    hookdescriptortest.h \
    hookssettingstest.h \
    settingstest.h

include(../../../share/config/settings/settings.pri)
include(../../mockups/mockups.pri)
