CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(sumo_export)
INCLUDEPATH += ../../.. \
    ../../../netEditor/src \
    ../../../netEditor/src/objects \
    ../../../netEditor/src/gui
HEADERS += \
    ../../../netEditor/src/gui/graphicsScene.h \
    ../../../netEditor/src/objects/lane.h \
    ../../../netEditor/src/objects/point.h \
    ../../../netEditor/src/objects/segment.h \
    ../../../netEditor/src/objects/object.h \
    sumo_export.h \
    edges_writer.h \
    nodes_writer.h \
    sumowriter.h \
    connections_writer.h \
    demand_writer.h \
    cfg_writer.h \
    induction_loops_sumo.h
SOURCES += \
    ../../../netEditor/src/objects/point.cpp \
    ../../../netEditor/src/objects/segment.cpp \
    ../../../netEditor/src/gui/graphicsScene.cpp \
    ../../../netEditor/src/objects/lane.cpp \
    ../../../netEditor/src/common.cpp \
    ../../../netEditor/src/objects/manouvre.cpp \
    ../../../netEditor/src/objects/road.cpp \
    ../../../netEditor/src/objects/object.cpp \
    sumo_export.cpp \
    edges_writer.cpp \
    nodes_writer.cpp \
    sumowriter.cpp \
    connections_writer.cpp \
    demand_writer.cpp \
    cfg_writer.cpp \
    induction_loops_sumo.cpp
DESTDIR = ../../../netEditor/plugins
