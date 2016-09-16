HEADERS += \
    $$PWD/sshconfig.h \
    $$PWD/jobconfig.h \
    $$PWD/hooksconfig.h \
    $$PWD/basejobconfig.h

SOURCES += \
    $$PWD/sshconfig.cpp \
    $$PWD/hooksconfig.cpp \
    $$PWD/basejobconfig.cpp

include(settings/settings.pri)
