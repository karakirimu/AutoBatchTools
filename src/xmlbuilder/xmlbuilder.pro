#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T23:48:45
#
#-------------------------------------------------

QT       -= gui

TARGET = xmlbuilder
TEMPLATE = lib

include($$PWD/../build_library_path.pri)

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
