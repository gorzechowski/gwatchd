QT -= gui
QT += testlib websockets

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = notificationTest

INCLUDEPATH += ../../../core/

HEADERS += \
    statusnotificationtest.h

SOURCES += \
    main.cpp \
    statusnotificationtest.cpp

include(../../../core/notification/notification.pri)
