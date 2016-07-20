QT -= gui
QT += testlib websockets

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = notificationTest

INCLUDEPATH += ../../../core/

HEADERS += \
    statenotificationtest.h

SOURCES += \
    main.cpp \
    statenotificationtest.cpp

include(../../../core/notification/notification.pri)
include(../../../core/socket/socket.pri)
