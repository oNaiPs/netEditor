CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(example_import)
INCLUDEPATH += ../../.. \
../../../app/src
HEADERS += example_import.h
SOURCES += example_import.cpp
DESTDIR = ../../../netEditor/plugins

INCLUDEPATH += ../../../app \
               ../../../app/src \
               ../../../app/src/objects \
               ../../../app/src/gui
