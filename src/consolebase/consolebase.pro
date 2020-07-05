#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T23:50:52
#
#-------------------------------------------------

QT       -= gui
QT       += widgets

# Prevent automatic conversion of dll name
CONFIG += skip_target_version_ext

TARGET = consolebase
TEMPLATE = lib
VERSION = 1.0.0

DEFINES += CONSOLEBASE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../build_library_path.pri)

SOURCES += \
        consolebase.cpp

HEADERS += \
        consolebase.h \
        consolebase_global.h 

LIBS += -L$$DESTDIR -lexecutor

unix {
    target.path = /usr/lib
    INSTALLS += target
}
