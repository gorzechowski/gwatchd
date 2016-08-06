include(../common.pri)

TARGET = synchronizejob

SOURCES += synchronizejob.cpp

HEADERS += synchronizejob.h

include(command/command.pri)
include(notification/notification.pri)

OTHER_FILES += \
    synchronizejob.json
