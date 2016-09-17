include(decorator/decorator.pri)
include(factory/factory.pri)

HEADERS += \
    $$PWD/filelogger.h \
    $$PWD/logger.h \
    $$PWD/simplelogger.h \
    $$PWD/loggercomposite.h

SOURCES += \
    $$PWD/filelogger.cpp \
    $$PWD/simplelogger.cpp \
    $$PWD/loggercomposite.cpp
