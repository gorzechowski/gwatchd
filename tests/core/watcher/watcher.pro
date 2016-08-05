include(../../common.pri)

TARGET = watcherTest

HEADERS += \
    watchertest.h

SOURCES += \
    main.cpp \
    watchertest.cpp

include(../../mockups/mockups.pri)
include(../../../core/watcher/watcher.pri)
