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
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../build_library_path.pri)

SOURCES += \
        filesearchdialog.cpp \
        searchdialogtable.cpp

HEADERS += \
        filesearchdialog.h \
        filesearchdialog_global.h \
        searchdialogtable.h

FORMS   += filesearchdialog.ui

RESOURCES += \
    ../../res/resources.qrc

LIBS += \
    -L$$DESTDIR -lxmlbuilder \
    -L$$DESTDIR -lsearchxmlbuilder \
    -L$$DESTDIR -lbasictable \
    -L$$DESTDIR -lfileinfodialog \
    -L$$DESTDIR -lfilesearchloader

unix {
    target.path = /usr/lib
    INSTALLS += target
}
