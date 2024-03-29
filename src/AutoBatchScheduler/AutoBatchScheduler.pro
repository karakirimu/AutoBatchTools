#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T01:06:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBatchScheduler
TEMPLATE = app

VERSION = 1.0.0
DEFINES += CURRENT_VERSION=\"\\\"$${VERSION}\\\"\"

win32 {
    QMAKE_TARGET_COMPANY = karakirimu
    QMAKE_TARGET_PRODUCT = AutoBatchScheduler
    QMAKE_TARGET_DESCRIPTION = AutoBatchScheduler
    QMAKE_TARGET_COPYRIGHT = Copyright 2016-2021 karakirimu
    RC_ICONS = ../../res/app_icons/app_abs.ico
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = $$OUT_PWD/../../
OBJECTS_DIR = $$OUT_PWD/../../abs/.obj
MOC_DIR = $$OUT_PWD/../../abs/.moc
RCC_DIR = $$OUT_PWD/../../abs/.rcc
UI_DIR = $$OUT_PWD/../../abs/.ui

INCLUDEPATH += $$OUT_PWD/../../library/
DEPENDPATH += $$OUT_PWD/../../library/
LIBRARYPATH = $$OUT_PWD/../../library/

SOURCES += \
        aboutabs.cpp \
        main.cpp \
        mainscheduler.cpp \
        startuptable.cpp \
        startupdialog.cpp

HEADERS += \
        aboutabs.h \
        mainscheduler.h \
        settingconstant.h \
        startuptable.h \
        startupdialog.h

FORMS += \
        aboutabs.ui \
        mainscheduler.ui \
        startupdialog.ui

RESOURCES += \
    ../../res/resources.qrc

include(taskscheduler/taskscheduler.pri)
include(systemtray/systemtray.pri)
include(statusarea/statusarea.pri)
include(settingcache/settingcache.pri)

LIBS += \
    -L$$LIBRARYPATH -lxmlbuilder \
    -L$$LIBRARYPATH -lbasictable \
    -L$$LIBRARYPATH -lprofilexmlbuilder \
    -L$$LIBRARYPATH -lprofilecombobox \
    -L$$LIBRARYPATH -lexecutor \
    -L$$LIBRARYPATH -lconsolebase

unix {
    LIBS+= \
        -L$$LIBRARYPATH -lstringxmlbuilder \
        -L$$LIBRARYPATH -lsearchxmlbuilder \
        -L$$LIBRARYPATH -lprocessxmlbuilder \
        -L$$LIBRARYPATH -lfilesearchloader
}
