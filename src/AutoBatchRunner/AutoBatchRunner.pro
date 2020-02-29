#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T23:04:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBatchRunner
TEMPLATE = app

TRANSLATIONS = abrunner_ja_JP.ts

VERSION = 1.0.0

win32 {
    QMAKE_TARGET_COMPANY = karakirimu
    QMAKE_TARGET_PRODUCT = AutoBatchRunner - ProfileRunner
    QMAKE_TARGET_DESCRIPTION = Execution of apro file
    QMAKE_TARGET_COPYRIGHT = Copyright 2016-2020 karakirimu
    RC_ICONS = ../../res/app_icons/app_abr.ico
}

DESTDIR = $$OUT_PWD/../../
OBJECTS_DIR = $$OUT_PWD/../../abr/.obj
MOC_DIR = $$OUT_PWD/../../abr/.moc
RCC_DIR = $$OUT_PWD/../../abr/.rcc
UI_DIR = $$OUT_PWD/../../abr/.ui

INCLUDEPATH += $$OUT_PWD/../../library/
DEPENDPATH += $$OUT_PWD/../../library/
LIBRARYPATH = $$OUT_PWD/../../library/

SOURCES += main.cpp\
    basedockwidget.cpp \
    autobatchrunner.cpp \
    entrytask.cpp \
    multitask.cpp

HEADERS  += \
    basedockwidget.h \
    autobatchrunner.h \
    entrytask.h \
    multitask.h

FORMS    += \
    autobatchrunner.ui

RESOURCES += \
    ../../res/resources.qrc

#include path
#include(xml/xml.pri)
#include(combobox/combobox.pri)
include(table/table.pri)
#include(timemanager/timemanager.pri)
#include(systemtray/systemtray.pri)
include(textedit/textedit.pri)
include(dialog/dialog.pri)

LIBS += \
    -L$$LIBRARYPATH -lVariantConverter \
    -L$$LIBRARYPATH -lxmlbuilder \
    -L$$LIBRARYPATH -lstringxmlbuilder \
    -L$$LIBRARYPATH -lsearchxmlbuilder \
    -L$$LIBRARYPATH -lpluginsxmlbuilder \
    -L$$LIBRARYPATH -lprocessxmlbuilder \
    -L$$LIBRARYPATH -lprofilexmlbuilder \
    -L$$LIBRARYPATH -lprofilecombobox \
    -L$$LIBRARYPATH -lbasictable \
    -L$$LIBRARYPATH -lFileQueueTable \
    -L$$LIBRARYPATH -lsearchtable \
    -L$$LIBRARYPATH -lstringtable \
    -L$$LIBRARYPATH -lextrafunctionstable \
    -L$$LIBRARYPATH -lfileinfodialog \
    -L$$LIBRARYPATH -lfilesearchdialog \
    -L$$LIBRARYPATH -lfilesearchloader \
    -L$$LIBRARYPATH -lconsolebase \
    -L$$LIBRARYPATH -lpluginstree \
    -L$$LIBRARYPATH -lexecutor
