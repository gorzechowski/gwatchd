include(factory/factory.pri)
include(notifier/notifier.pri)

HEADERS += \
    $$PWD/notification.h \
    $$PWD/notificationmanager.h \
    $$PWD/payload.h \
    $$PWD/statenotification.h \
    $$PWD/jobsnotificationmanager.h

SOURCES += \
    $$PWD/notificationmanager.cpp \
    $$PWD/statenotification.cpp \
    $$PWD/jobsnotificationmanager.cpp
