QT += core
QT -= gui

TEMPLATE = lib

CONFIG   += plugin

unix:!macx {
    QMAKE_RPATHDIR = \$\$ORIGIN/../libs
    RPATH = $$join(QMAKE_RPATHDIR, ":")

    QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,$${RPATH}\'
    QMAKE_RPATHDIR =
}

INCLUDEPATH += \
    $$PWD/../core/ \
#    $$PWD/../libs/yaml-cpp/include \
    /usr/local/include

#LIBS += -L$$OUT_PWD/../../libs/yaml-cpp -lyaml-cpp
