include(../common.pri)

TARGET = commandjob

SOURCES += commandjob.cpp

HEADERS += commandjob.h

include(config/config.pri)
include(command/command.pri)

OTHER_FILES += \
    commandjob.json
