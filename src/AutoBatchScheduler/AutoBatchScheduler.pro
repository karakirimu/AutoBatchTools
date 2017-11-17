#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T01:06:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBatchScheduler
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainscheduler.cpp \
        startupxmlbuilder.cpp \
        startuptable.cpp \
        startupdialog.cpp

HEADERS += \
        mainscheduler.h \
        startupxmlbuilder.h \
        startuptable.h \
        startupdialog.h

FORMS += \
        mainscheduler.ui \
        startupdialog.ui

RESOURCES += \
    ../../res/resources.qrc

include(taskscheduler/taskscheduler.pri)
include(systemtray/systemtray.pri)
include(statusarea/statusarea.pri)

Release:DESTDIR = $$PWD/../../build/release
Release:OBJECTS_DIR = $$PWD/../../build/release/AutoBatchScheduler/.obj
Release:MOC_DIR = $$PWD/../../build/release/AutoBatchScheduler/.moc
Release:RCC_DIR = $$PWD/../../build/release/AutoBatchScheduler/.rcc
Release:UI_DIR = $$PWD/../../build/release/AutoBatchScheduler/.ui

Debug:DESTDIR = $$PWD/../../build/debug
Debug:OBJECTS_DIR = $$PWD/../../build/debug/AutoBatchScheduler/.obj
Debug:MOC_DIR = $$PWD/../../build/debug/AutoBatchScheduler/.moc
Debug:RCC_DIR = $$PWD/../../build/debug/AutoBatchScheduler/.rcc
Debug:UI_DIR = $$PWD/../../build/debug/AutoBatchScheduler/.ui

unix|win32:CONFIG(debug, debug|release) LIBS += \
            -L$$PWD/../../build/debug/libs/ -lVariantConverter \
            -L$$PWD/../../build/debug/libs/ -lxmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lbasictable \
            -L$$PWD/../../build/debug/libs/ -lprofilexmlbuilder \
            -L$$PWD/../../build/debug/libs/ -lprofilecombobox \
            -L$$PWD/../../build/debug/libs/ -lexecutor
