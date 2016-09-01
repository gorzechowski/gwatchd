include(../common.pri)

TARGET = synchronizejob

SOURCES += synchronizejob.cpp

HEADERS += synchronizejob.h

include(config/config.pri)
include(command/command.pri)
include(notification/notification.pri)

OTHER_FILES += \
    synchronizejob.json
