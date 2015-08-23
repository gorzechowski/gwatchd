QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = watcherTest

INCLUDEPATH += ../../../core/

HEADERS += \
    watchertest.h
#    ../../../core/watcher/watcher.h

SOURCES += \
    main.cpp \
    watchertest.cpp
#    ../../../core/watcher/watcher.cpp

#linux {
#    HEADERS += \
#        ../../../core/watcher/inotifythread.h

#    SOURCES += \
#        ../../../core/watcher/inotifythread.cpp
#} else:macx|freebsd|openbsd|netbsd {
#    HEADERS += \
#        ../../../core/watcher/kqueuethread.h

#    SOURCES += \
#        ../../../core/watcher/kqueuethread.cpp
#}

include(../../mockups/mockups.pri)
include(../../../core/watcher/watcher.pri)
