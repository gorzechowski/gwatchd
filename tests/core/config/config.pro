QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = configTest

SOURCES += \
    main.cpp \
    yamlconfigtest.cpp

HEADERS += \
    yamlconfigtest.h

include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)

INCLUDEPATH += \
    ../../../core/ \
    $$PWD/../../../libs/yaml-cpp/include

QMAKE_RPATHDIR += /usr/lib/gwatchd/

LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp
