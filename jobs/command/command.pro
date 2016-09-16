include(../common.pri)

TARGET = commandjob

SOURCES += commandjob.cpp

HEADERS += commandjob.h

include(notification/notification.pri)
include(config/config.pri)
include(../../share/share.pri)

OTHER_FILES += \
    commandjob.json
