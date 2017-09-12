#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T17:27:13
#
#-------------------------------------------------

QT       += widgets

TARGET = filesearchdialog
TEMPLATE = lib

DEFINES += FILESEARCHDIALOG_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

SOURCES += \
        filesearchdialog.cpp \
        searchdialogtable.cpp

HEADERS += \
        filesearchdialog.h \
        filesearchdialog_global.h \
        searchdialogtable.h

FORMS   += filesearchdialog.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    ../../res/resources.qrc

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lVariantConverter

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lxmlbuilder

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lsearchxmlbuilder

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lfilesearchloader

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lfileinfodialog

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lbasictable

INCLUDEPATH += $$PWD/../../build/debug/libs
DEPENDPATH += $$PWD/../../build/debug/libs
