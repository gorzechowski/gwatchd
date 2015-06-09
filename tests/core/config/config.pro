QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = configTest

SOURCES += \
    main.cpp \
    yamlconfigtest.cpp \
    ../../../core/config/yamlconfig.cpp

HEADERS += \
    yamlconfigtest.h \
    ../../../core/config/yamlconfig.h \
    ../../../core/config/config.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    ../../../core/ \
    $$PWD/../../../libs/yaml-cpp/include

QMAKE_RPATHDIR += /usr/lib/gwatchd/

LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp

OTHER_FILES += \
    config.yml
