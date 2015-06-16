CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(feup_simulator_export)

INCLUDEPATH += ../../../app \
               ../../../app/src \
               ../../../app/src/objects \
               ../../../app/src/gui

HEADERS += \
            ../../../app/src/gui/graphicsScene.h \
            ../../../app/src/objects/lane.h \
            ../../../app/src/objects/segment.h \
            ../../../app/src/objects/point.h \
            ../../../app/src/objects/object.h \
            feup_simulator_export.h \
            realRoad.h \
            XMLNodeWriter.h \
            XMLRoadWriter.h \
            XMLTripAssignmentWriter.h
SOURCES += \
            ../../../app/src/objects/point.cpp \
            ../../../app/src/objects/lane.cpp \
            ../../../app/src/objects/road.cpp \
            ../../../app/src/objects/manouvre.cpp \
            ../../../app/src/objects/segment.cpp \
            ../../../app/src/common.cpp \
            ../../../app/src/gui/graphicsScene.cpp \
            ../../../app/src/objects/object.cpp \
           XMLNodeWriter.cpp \
           XMLRoadWriter.cpp \
           XMLTripAssignmentWriter.cpp \
           realRoad.cpp \
           feup_simulator_export.cpp
DESTDIR = ../../../app/plugins
