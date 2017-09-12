#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T23:48:45
#
#-------------------------------------------------

QT       -= gui

TARGET = xmlbuilder
TEMPLATE = lib

Release:DESTDIR = $$PWD/../../build/release/libs
Release:OBJECTS_DIR = $$PWD/../../build/release/.obj
Release:MOC_DIR = $$PWD/../../build/release/.moc
Release:RCC_DIR = $$PWD/../../build/release/.rcc
Release:UI_DIR = $$PWD/../../build/release/.ui

Debug:DESTDIR = $$PWD/../../build/debug/libs
Debug:OBJECTS_DIR = $$PWD/../../build/debug/.obj
Debug:MOC_DIR = $$PWD/../../build/debug/.moc
Debug:RCC_DIR = $$PWD/../../build/debug/.rcc
Debug:UI_DIR = $$PWD/../../build/debug/.ui

#include(localloader/localloader.pri)

SOURCES += xmlbuilder.cpp\
        basexmlbuilder.cpp

HEADERS += xmlbuilder_global.h\
        xmlbuilder.h\
        basexmlbuilder.h

DEFINES += XMLBUILDER_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}
