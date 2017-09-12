#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T17:42:48
#
#-------------------------------------------------

QT       -= gui

TARGET = filesearchloader
TEMPLATE = lib

DEFINES += FILESEARCHLOADER_LIBRARY

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
        filesearchloader.cpp \
        basefilesearch.cpp

HEADERS += \
        filesearchloader.h \
        filesearchloader_global.h \
        basefilesearch.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/debug/libs/ -lxmlbuilder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/debug/libs/ -lxmlbuilder
else:unix: LIBS += -L$$PWD/../../build/debug/libs/ -lxmlbuilder

INCLUDEPATH += $$PWD/../../build/debug/libs
DEPENDPATH += $$PWD/../../build/debug/libs

unix|win32: LIBS += -L$$PWD/../../build/debug/libs/ -lsearchxmlbuilder

INCLUDEPATH += $$PWD/../../build/debug/libs
DEPENDPATH += $$PWD/../../build/debug/libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/debug/libs/ -lVariantConverter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/debug/libs/ -lVariantConverter
else:unix: LIBS += -L$$PWD/../../build/debug/libs/ -lVariantConverter

INCLUDEPATH += $$PWD/../../build/debug/libs
DEPENDPATH += $$PWD/../../build/debug/libs
