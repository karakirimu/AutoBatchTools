#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T16:36:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProfileEditor
TEMPLATE = app

Release:DESTDIR = $$PWD/../../build/release
Release:OBJECTS_DIR = $$PWD/../../build/release/ProfileEditor/.obj
Release:MOC_DIR = $$PWD/../../build/release/ProfileEditor/.moc
Release:RCC_DIR = $$PWD/../../build/release/ProfileEditor/.rcc
Release:UI_DIR = $$PWD/../../build/release/ProfileEditor/.ui

Debug:DESTDIR = $$PWD/../../build/debug
Debug:OBJECTS_DIR = $$PWD/../../build/debug/ProfileEditor/.obj
Debug:MOC_DIR = $$PWD/../../build/debug/ProfileEditor/.moc
Debug:RCC_DIR = $$PWD/../../build/debug/ProfileEditor/.rcc
Debug:UI_DIR = $$PWD/../../build/debug/ProfileEditor/.ui

HEADERS  += $$PWD/profileeditor.h \
    $$PWD/profiletreewidget.h \
    $$PWD/pesharedfunction.h \
    $$PWD/fileoperationsignalbinder.h \
    $$PWD/profilexmlbuilder.h \
    basedockwidget.h \
    runtasksignalbinder.h

SOURCES += $$PWD/profileeditor.cpp \
    $$PWD/profiletreewidget.cpp \
    $$PWD/pesharedfunction.cpp \
    $$PWD/fileoperationsignalbinder.cpp \
    $$PWD/profilexmlbuilder.cpp \
    basedockwidget.cpp \
    runtasksignalbinder.cpp

SOURCES += main.cpp

FORMS    += $$PWD/profileeditor.ui

RESOURCES += \
    $$PWD/../../res/resources.qrc

include(graphicsarea/graphicsarea.pri)
include(editarea/editarea.pri)
include(variantarea/variantarea.pri)
include(settingdialog/settingdialog.pri)
include(testarea/testarea.pri)

unix|win32: LIBS += \
            -L$$PWD/../../build/debug/libs/ -lVariantConverter \
            -L$$PWD/../../build/debug/libs/ -lconvertchecker \
            -L$$PWD/../../build/debug/libs/ -lxmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lstringxmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lsearchxmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lextrafunctionsxmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lprocessxmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lbasictable \
            -L$$PWD/../../build/debug/libs/ -lFileQueueTable \
            -L$$PWD/../../build/debug/libs/ -lsearchtable \
            -L$$PWD/../../build/debug/libs/ -lstringtable \
            -L$$PWD/../../build/debug/libs/ -lfileinfodialog \
            -L$$PWD/../../build/debug/libs/ -lfilesearchdialog \
            -L$$PWD/../../build/debug/libs/ -lextrafunctionstable \
            -L$$PWD/../../build/debug/libs/ -lexecutor \


INCLUDEPATH += $$PWD/../../build/debug/libs
DEPENDPATH += $$PWD/../../build/debug/libs
