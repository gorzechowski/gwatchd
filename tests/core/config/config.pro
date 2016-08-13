include(../../common.pri)

TARGET = configTest

SOURCES += \
    main.cpp \
    yamlconfigtest.cpp

HEADERS += \
    yamlconfigtest.h

include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)

LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp
