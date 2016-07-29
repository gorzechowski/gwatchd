QT       += core
QT       -= gui

TEMPLATE = lib

CONFIG   += plugin

TARGET = $$qtLibraryTarget(synchronizejob)

INCLUDEPATH += \
    ../../core/ \
    $$PWD/../../libs/yaml-cpp/include


QMAKE_RPATHDIR += /usr/lib/gwatchd/

LIBS += -L$$OUT_PWD/../../libs/yaml-cpp -lyaml-cpp

SOURCES += synchronizejob.cpp

HEADERS += synchronizejob.h

include(command/command.pri)
include(notification/notification.pri)

OTHER_FILES += \
    synchronizejob.json
