include(../../common.pri)

QT += websockets

TARGET = notificationTest

HEADERS += \
    statenotificationtest.h

SOURCES += \
    main.cpp \
    statenotificationtest.cpp

include(../../../core/notification/notification.pri)
include(../../../core/socket/socket.pri)
