include(../../common.pri)

TARGET = jobTest

SOURCES += \
    main.cpp \
    jobdescriptortest.cpp \
    jobscollectortest.cpp

HEADERS += \
    jobdescriptortest.h \
    jobscollectortest.h

include(../../../core/config/config.pri)
include(../../../core/job/job.pri)
include(../../../core/logger/logger.pri)
include(../../mockups/mockups.pri)
