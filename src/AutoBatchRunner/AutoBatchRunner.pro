#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T23:04:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBatchRunner
TEMPLATE = app

VERSION = 1.0.0
DEFINES += CURRENT_VERSION=\"\\\"$${VERSION}\\\"\"

win32 {
    QMAKE_TARGET_COMPANY = karakirimu
    QMAKE_TARGET_PRODUCT = AutoBatchRunner
    QMAKE_TARGET_DESCRIPTION = AutoBatchRunner
    QMAKE_TARGET_COPYRIGHT = Copyright 2016-2021 karakirimu
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
    multitask.h \
    settingconstant.h

FORMS    += \
    autobatchrunner.ui

RESOURCES += \
    ../../res/resources.qrc

include($$PWD/../share/share.pri)
include(table/table.pri)
include(textedit/textedit.pri)
include(dialog/dialog.pri)

LIBS += \
    -L$$LIBRARYPATH -lxmlbuilder \
    -L$$LIBRARYPATH -lstringxmlbuilder \
    -L$$LIBRARYPATH -lsearchxmlbuilder \
    -L$$LIBRARYPATH -lpluginsxmlbuilder \
    -L$$LIBRARYPATH -lprocessxmlbuilder \
    -L$$LIBRARYPATH -lprofilexmlbuilder \
    -L$$LIBRARYPATH -lprofilecombobox \
    -L$$LIBRARYPATH -lbasictable \
    -L$$LIBRARYPATH -lfilequeuetable \
    -L$$LIBRARYPATH -lsearchtable \
    -L$$LIBRARYPATH -lstringtable \
    -L$$LIBRARYPATH -lfileinfodialog \
    -L$$LIBRARYPATH -lfilesearchdialog \
    -L$$LIBRARYPATH -lfilesearchloader \
    -L$$LIBRARYPATH -lconsolebase \
    -L$$LIBRARYPATH -lpluginstree \
    -L$$LIBRARYPATH -lexecutor
