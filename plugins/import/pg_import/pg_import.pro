CONFIG += plugin
TEMPLATE = lib
TARGET = $$qtLibraryTarget(pg_import)
INCLUDEPATH += ../../../netEditor \
               ../../../netEditor/src \
               ../../../netEditor/src/objects \
               ../../../netEditor/src/gui


unix:INCLUDEPATH += /usr/include/postgresql

HEADERS += pg_import.h pgconnection_gui.h
SOURCES += pg_import.cpp pgconnection_gui.cpp
FORMS += pgconnection_gui.ui

win32:LIBS += "C:/Programas/PostgreSQL/8.4/lib/libpq.dll"
unix:LIBS += -lpq
win32:INCLUDEPATH += "C:\Programas\PostgreSQL\8.4\include"

DESTDIR = ../../../app/plugins
