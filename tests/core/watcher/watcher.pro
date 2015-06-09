QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = watcherTest

SOURCES += \
    inotifythreadtest.cpp \
    ../../../core/watcher/inotify/inotifythread.cpp \
    main.cpp

HEADERS += \
    inotifythreadtest.h \
    ../../../core/watcher/inotify/inotifythread.h

INCLUDEPATH += ../../../core/
