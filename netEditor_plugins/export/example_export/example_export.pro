CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(example_export)
HEADERS += example_export.h
SOURCES += example_export.cpp
DESTDIR = ../../../netEditor/plugins
INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui
