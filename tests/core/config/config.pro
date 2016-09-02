include(../../common.pri)

TARGET = configTest

SOURCES += \
    main.cpp \
#    yamlconfigtest.cpp \
    jsonconfigtest.cpp

HEADERS += \
#    yamlconfigtest.h \
    jsonconfigtest.h

include(../../../core/config/config.pri)
include(../../mockups/mockups.pri)

#LIBS += -L$$OUT_PWD/../../../libs/yaml-cpp -lyaml-cpp
