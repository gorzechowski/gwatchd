include(../../common.pri)

TARGET = configTest

SOURCES += \
    main.cpp \
    jsonconfigtest.cpp

HEADERS += \
    jsonconfigtest.h

include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)
