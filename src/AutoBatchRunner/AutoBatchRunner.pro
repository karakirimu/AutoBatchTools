#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T23:04:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBatchRunner
TEMPLATE = app

Release:DESTDIR = $$PWD/../../build/release
Release:OBJECTS_DIR = $$PWD/../../build/release/AutoBatchRunner/.obj
Release:MOC_DIR = $$PWD/../../build/release/AutoBatchRunner/.moc
Release:RCC_DIR = $$PWD/../../build/release/AutoBatchRunner/.rcc
Release:UI_DIR = $$PWD/../../build/release/AutoBatchRunner/.ui

Debug:DESTDIR = $$PWD/../../build/debug
Debug:OBJECTS_DIR = $$PWD/../../build/debug/AutoBatchRunner/.obj
Debug:MOC_DIR = $$PWD/../../build/debug/AutoBatchRunner/.moc
Debug:RCC_DIR = $$PWD/../../build/debug/AutoBatchRunner/.rcc
Debug:UI_DIR = $$PWD/../../build/debug/AutoBatchRunner/.ui

SOURCES += main.cpp\
        mainwindow.cpp \
    basedockwidget.cpp

HEADERS  += mainwindow.h \
    basedockwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    ../../res/resources.qrc

#include path
include(xml/xml.pri)
include(combobox/combobox.pri)
include(table/table.pri)
include(timemanager/timemanager.pri)
include(systemtray/systemtray.pri)
include(textedit/textedit.pri)
include(dialog/dialog.pri)

unix|win32:CONFIG(debug, debug|release) LIBS += \
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
            -L$$PWD/../../build/debug/libs/ -lextrafunctionstable \
            -L$$PWD/../../build/debug/libs/ -lfileinfodialog \
            -L$$PWD/../../build/debug/libs/ -lfilesearchdialog \
            -L$$PWD/../../build/debug/libs/ -lfilesearchloader \
            -L$$PWD/../../build/debug/libs/ -lexecutor \

INCLUDEPATH += \
        $$PWD/../../build/debug/libs \

DEPENDPATH += \
        $$PWD/../../build/debug/libs \
