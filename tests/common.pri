QT -= gui
QT += testlib

TEMPLATE = app

CONFIG += console testcase
CONFIG -= app_bundle

INCLUDEPATH += \
    $$PWD/../core/ \
    $$PWD/../share/ \
    /usr/local/include
