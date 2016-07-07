QT       += core websockets
QT       -= gui

TEMPLATE = app

TARGET = gwatchd

CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += $$PWD/../libs/yaml-cpp/include

QMAKE_RPATHDIR += /usr/lib/gwatchd/

LIBS += -L$$OUT_PWD/../libs/yaml-cpp -lyaml-cpp

SOURCES += main.cpp

include(config/config.pri)
include(job/job.pri)
include(watcher/watcher.pri)
include(logger/logger.pri)
include(notification/notification.pri)
