CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(gdal_import)
DEFINES+=QT_NO_DEBUG_STREAM

INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui

unix:INCLUDEPATH += /usr/include/gdal

HEADERS += gdal_import.h \
    ../../../netEditor/src/gui/graphicsScene.h \
    ../../../netEditor/src/objects/lane.h \
    ../../../netEditor/src/objects/point.h \
    ../../../netEditor/src/objects/object.h \
    ../../../netEditor/src/objects/segment.h


SOURCES += gdal_import.cpp \
    ../../../netEditor/src/objects/point.cpp \
    ../../../netEditor/src/objects/segment.cpp \
    ../../../netEditor/src/objects/lane.cpp \
    ../../../netEditor/src/objects/object.cpp \
    ../../../netEditor/src/gui/graphicsScene.cpp \
    ../../../netEditor/src/common.cpp
DESTDIR = ../../../netEditor/plugins
unix:LIBS += -lgdal1.7.0
win32: LIBS+= c:/gdal/bin/gdal17.dll

