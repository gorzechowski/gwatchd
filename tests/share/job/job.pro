include(../../common.pri)

TARGET = jobTest

SOURCES += \
    main.cpp \
    entrylisttest.cpp

HEADERS += \
    entrylisttest.h

include(../../../share/job/job.pri)
include(../../../share/config/settings/settings.pri)
include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)
