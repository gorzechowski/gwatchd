include(../common.pri)

TARGET = commandjob

SOURCES += commandjob.cpp

HEADERS += commandjob.h

include(config/config.pri)
include(command/command.pri)
include(notification/notification.pri)

OTHER_FILES += \
    commandjob.json
