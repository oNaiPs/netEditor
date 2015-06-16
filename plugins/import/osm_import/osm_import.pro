CONFIG += plugin
unix:LIBS += -lGeographic
QT += network  webkit
TEMPLATE = lib
TARGET = $$qtLibraryTarget(osm_import)

INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui

HEADERS += osm_import.h \
    osm_reader.h \
    ../../../netEditor/src/objects/point.h \
    ../../../netEditor/src/objects/segment.h \
    ../../../netEditor/src/gui/graphicsScene.h \
    ../../../netEditor/src/objects/lane.h \
../../../netEditor/src/objects/object.h \
    ../../../netEditor/src/config.h \
    ../../../netEditor/src/common.h

SOURCES += osm_import.cpp \
    osm_reader.cpp \
    ../../../netEditor/src/objects/point.cpp \
    ../../../netEditor/src/objects/segment.cpp \
    ../../../netEditor/src/objects/object.cpp \
    ../../../netEditor/src/gui/graphicsScene.cpp \
    ../../../netEditor/src/objects/lane.cpp \
    ../../../netEditor/src/common.cpp

RESOURCES += \
    resources.qrc

DESTDIR = ../../../app/plugins
