HEADERS += \
    $$PWD/sshsettings.h \
    $$PWD/hookdescriptor.h \
    $$PWD/hookssettings.h

SOURCES += \
    $$PWD/sshsettings.cpp \
    $$PWD/hookdescriptor.cpp \
    $$PWD/hookssettings.cpp

include(factory/factory.pri)
