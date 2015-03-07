CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(example_import)
INCLUDEPATH += ../../.. \
../../../netEditor/src
HEADERS += example_import.h
SOURCES += example_import.cpp
DESTDIR = ../../../netEditor/plugins

INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui
