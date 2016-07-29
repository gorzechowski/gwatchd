HEADERS += \
    logger/decorator/loggertimestampdecorator.h \
    logger/filelogger.h \
    logger/logger.h \
    $$PWD/simplelogger.h \
    $$PWD/loggercomposite.h

SOURCES += \
    logger/decorator/loggertimestampdecorator.cpp \
    logger/filelogger.cpp \
    $$PWD/simplelogger.cpp \
    $$PWD/loggercomposite.cpp
