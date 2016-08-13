HEADERS += \
    $$PWD/watcher.h

SOURCES += \
    $$PWD/watcher.cpp

linux {
    HEADERS += \
        $$PWD/inotifythread.h

    SOURCES += \
        $$PWD/inotifythread.cpp
} else:macx|freebsd|openbsd|netbsd {
    HEADERS += \
        $$PWD/kqueuethread.h

    SOURCES += \
        $$PWD/kqueuethread.cpp
}
