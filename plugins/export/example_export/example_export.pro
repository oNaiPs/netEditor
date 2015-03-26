CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(example_export)
HEADERS += example_export.h
SOURCES += example_export.cpp
DESTDIR = ../../../netEditor/plugins
INCLUDEPATH += ../../../app \
               ../../../app/src \
               ../../../app/src/objects \
               ../../../app/src/gui
