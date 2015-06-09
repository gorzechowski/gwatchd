QT -= gui core

TEMPLATE = lib

CONFIG   += plugin

TARGET = $$qtLibraryTarget(yaml-cpp)

SOURCES += \
    src/binary.cpp \
    src/convert.cpp \
    src/directives.cpp \
    src/emit.cpp \
    src/emitfromevents.cpp \
    src/emitter.cpp \
    src/emitterstate.cpp \
    src/emitterutils.cpp \
    src/exp.cpp \
    src/memory.cpp \
    src/node_data.cpp \
    src/node.cpp \
    src/nodebuilder.cpp \
    src/nodeevents.cpp \
    src/null.cpp \
    src/ostream_wrapper.cpp \
    src/parse.cpp \
    src/parser.cpp \
    src/regex_yaml.cpp \
    src/scanner.cpp \
    src/scanscalar.cpp \
    src/scantag.cpp \
    src/scantoken.cpp \
    src/simplekey.cpp \
    src/singledocparser.cpp \
    src/stream.cpp \
    src/tag.cpp \
    src/contrib/graphbuilder.cpp \
    src/contrib/graphbuilderadapter.cpp

HEADERS += \
    src/collectionstack.h \
    src/directives.h \
    src/emitterstate.h \
    src/emitterutils.h \
    src/exp.h \
    src/indentation.h \
    src/nodebuilder.h \
    src/nodeevents.h \
    src/ptr_stack.h \
    src/ptr_vector.h \
    src/regex_yaml.h \
    src/regeximpl.h \
    src/scanner.h \
    src/scanscalar.h \
    src/scantag.h \
    src/setting.h \
    src/singledocparser.h \
    src/stream.h \
    src/streamcharsource.h \
    src/stringsource.h \
    src/tag.h \
    src/token.h \
    src/contrib/graphbuilderadapter.h

INCLUDEPATH += $$PWD/include
