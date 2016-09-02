QT       += core websockets
QT       -= gui

TEMPLATE = app

TARGET = gwatchd

CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += \
    /usr/local/include

unix:!macx {
    QMAKE_RPATHDIR = \$\$ORIGIN/libs
    RPATH = $$join(QMAKE_RPATHDIR, ":")

    QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,$${RPATH}\'
    QMAKE_RPATHDIR =
}

SOURCES += main.cpp

include(application/application.pri)
include(config/config.pri)
include(job/job.pri)
include(watcher/watcher.pri)
include(logger/logger.pri)
include(notification/notification.pri)
include(socket/socket.pri)
