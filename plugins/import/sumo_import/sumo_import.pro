CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(sumo_import)
INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui
HEADERS += \
      ../../../netEditor/src/gui/graphicsScene.h \
      ../../../netEditor/src/objects/point.h \
      ../../../netEditor/src/objects/segment.h \
      ../../../netEditor/src/objects/lane.h \
      ../../../netEditor/src/objects/manouvre.h \
      ../../../netEditor/src/objects/object.h \
      sumo_import.h \
      edges_reader.h \
      nodes_reader.h \
      connections_reader.h
SOURCES += \
      ../../../netEditor/src/objects/point.cpp \
      ../../../netEditor/src/objects/road.cpp \
      ../../../netEditor/src/objects/lane.cpp \
      ../../../netEditor/src/objects/segment.cpp \
      ../../../netEditor/src/objects/manouvre.cpp \
      ../../../netEditor/src/objects/object.cpp \
      ../../../netEditor/src/gui/graphicsScene.cpp \
      ../../../netEditor/src/common.cpp \
      sumo_import.cpp \
      edges_reader.cpp \
      nodes_reader.cpp \
      connections_reader.cpp
DESTDIR = ../../../netEditor/plugins
