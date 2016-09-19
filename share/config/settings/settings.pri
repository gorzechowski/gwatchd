HEADERS += \
    $$PWD/sshsettings.h \
    $$PWD/hookdescriptor.h \
    $$PWD/hookssettings.h \
    $$PWD/settings.h

SOURCES += \
    $$PWD/sshsettings.cpp \
    $$PWD/hookdescriptor.cpp \
    $$PWD/hookssettings.cpp \
    $$PWD/settings.cpp

include(factory/factory.pri)
