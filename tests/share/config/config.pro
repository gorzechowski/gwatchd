include(../../common.pri)

TARGET = configTest

SOURCES += \
    main.cpp \
    sshsettingstest.cpp \
    hookdescriptortest.cpp \
    hookssettingstest.cpp

HEADERS += \
    sshsettingstest.h \
    hookdescriptortest.h \
    hookssettingstest.h

include(../../../share/config/settings/settings.pri)
include(../../mockups/mockups.pri)
