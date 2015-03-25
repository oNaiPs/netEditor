CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(sumo_export)
INCLUDEPATH += ../../.. \
    ../../../app/src \
    ../../../app/src/objects \
    ../../../app/src/gui
HEADERS += \
    ../../../app/src/gui/graphicsScene.h \
    ../../../app/src/objects/lane.h \
    ../../../app/src/objects/point.h \
    ../../../app/src/objects/segment.h \
    ../../../app/src/objects/object.h \
    sumo_export.h \
    edges_writer.h \
    nodes_writer.h \
    sumowriter.h \
    connections_writer.h \
    demand_writer.h \
    cfg_writer.h \
    induction_loops_sumo.h
SOURCES += \
    ../../../app/src/objects/point.cpp \
    ../../../app/src/objects/segment.cpp \
    ../../../app/src/gui/graphicsScene.cpp \
    ../../../app/src/objects/lane.cpp \
    ../../../app/src/common.cpp \
    ../../../app/src/objects/manouvre.cpp \
    ../../../app/src/objects/road.cpp \
    ../../../app/src/objects/object.cpp \
    sumo_export.cpp \
    edges_writer.cpp \
    nodes_writer.cpp \
    sumowriter.cpp \
    connections_writer.cpp \
    demand_writer.cpp \
    cfg_writer.cpp \
    induction_loops_sumo.cpp
DESTDIR = ../../../netEditor/plugins
