CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(sumo_import)
INCLUDEPATH += ../../../app \
               ../../../app/src \
               ../../../app/src/objects \
               ../../../app/src/gui
HEADERS += \
      ../../../app/src/gui/graphicsScene.h \
      ../../../app/src/objects/point.h \
      ../../../app/src/objects/segment.h \
      ../../../app/src/objects/lane.h \
      ../../../app/src/objects/manouvre.h \
      ../../../app/src/objects/object.h \
      sumo_import.h \
      edges_reader.h \
      nodes_reader.h \
      connections_reader.h
SOURCES += \
      ../../../app/src/objects/point.cpp \
      ../../../app/src/objects/road.cpp \
      ../../../app/src/objects/lane.cpp \
      ../../../app/src/objects/segment.cpp \
      ../../../app/src/objects/manouvre.cpp \
      ../../../app/src/objects/object.cpp \
      ../../../app/src/gui/graphicsScene.cpp \
      ../../../app/src/common.cpp \
      sumo_import.cpp \
      edges_reader.cpp \
      nodes_reader.cpp \
      connections_reader.cpp
DESTDIR = ../../../netEditor/plugins
