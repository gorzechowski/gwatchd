include(../common.pri)

TARGET = commandjob

SOURCES += commandjob.cpp

HEADERS += commandjob.h

include(config/config.pri)
include(notification/notification.pri)
include(../../share/share.pri)

OTHER_FILES += \
    commandjob.json
