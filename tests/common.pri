QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += \
    $$PWD/../core/ \
    $$PWD/../libs/yaml-cpp/include

QMAKE_RPATHDIR += $$OUT_PWD/../../../libs/yaml-cpp
