CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(feup_simulator_export)

INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui

HEADERS += \
            ../../../netEditor/src/GUI/graphicsScene.h \
            ../../../netEditor/src/objects/lane.h \
            ../../../netEditor/src/objects/segment.h \
            ../../../netEditor/src/objects/point.h \
            ../../../netEditor/src/objects/object.h \
            feup_simulator_export.h \
            realRoad.h \
            XMLNodeWriter.h \
            XMLRoadWriter.h \
            XMLTripAssignmentWriter.h
SOURCES += \
            ../../../netEditor/src/objects/point.cpp \
            ../../../netEditor/src/objects/lane.cpp \
            ../../../netEditor/src/objects/road.cpp \
            ../../../netEditor/src/objects/manouvre.cpp \
            ../../../netEditor/src/objects/segment.cpp \
            ../../../netEditor/src/common.cpp \
            ../../../netEditor/src/gui/graphicsScene.cpp \
            ../../../netEditor/src/objects/object.cpp \
           XMLNodeWriter.cpp \
           XMLRoadWriter.cpp \
           XMLTripAssignmentWriter.cpp \
           realRoad.cpp \
           feup_simulator_export.cpp
DESTDIR = ../../../netEditor/plugins


