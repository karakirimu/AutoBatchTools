#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T23:33:32
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stringtable
TEMPLATE = lib

DEFINES += STRINGTABLE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Release:DESTDIR = $$PWD/../../build/release/libs
#Release:OBJECTS_DIR = $$PWD/../../build/release/.obj
#Release:MOC_DIR = $$PWD/../../build/release/.moc
#Release:RCC_DIR = $$PWD/../../build/release/.rcc
#Release:UI_DIR = $$PWD/../../build/release/.ui

#Debug:DESTDIR = $$PWD/../../build/debug/libs
#Debug:OBJECTS_DIR = $$PWD/../../build/debug/.obj
#Debug:MOC_DIR = $$PWD/../../build/debug/.moc
#Debug:RCC_DIR = $$PWD/../../build/debug/.rcc
#Debug:UI_DIR = $$PWD/../../build/debug/.ui

SOURCES += \
        stringtable.cpp

HEADERS += \
        stringtable.h \
        stringtable_global.h 

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

#RESOURCES += \
#    ../../res/resources.qrc

#unix|win32:CONFIG(debug, debug|release) {
#    LIBS += \
#        -L$$PWD/../../build/debug/libs/ -lxmlbuilder \
#        -L$$PWD/../../build/debug/libs/ -lstringxmlbuilder \
#        -L$$PWD/../../build/debug/libs/ -lbasictable
#    INCLUDEPATH += $$PWD/../../build/debug/libs
#    DEPENDPATH += $$PWD/../../build/debug/libs
#}

#unix|win32:CONFIG(release, debug|release) {
#    LIBS += \
#        -L$$PWD/../../build/release/libs/ -lxmlbuilder \
#        -L$$PWD/../../build/release/libs/ -lstringxmlbuilder \
#        -L$$PWD/../../build/release/libs/ -lbasictable
#    INCLUDEPATH += $$PWD/../../build/release/libs
#    DEPENDPATH += $$PWD/../../build/release/libs
#}

include($$PWD/../build_library_path.pri)

RESOURCES += \
    ..\..\res\resources.qrc

LIBS += \
    -L$$DESTDIR -lxmlbuilder \
    -L$$DESTDIR -lstringxmlbuilder \
    -L$$DESTDIR -lbasictable

#win32 {
##    Release:DESTDIR = $$PWD\..\..\build\release\libs
##    Release:OBJECTS_DIR = $$PWD\..\..\build\release\.obj
##    Release:MOC_DIR = $$PWD\..\..\build\release\.moc
##    Release:RCC_DIR = $$PWD\..\..\build\release\.rcc
##    Release:UI_DIR = $$PWD\..\..\build\release\.ui

##    Debug:DESTDIR = $$PWD\..\..\build\debug\libs
##    Debug:OBJECTS_DIR = $$PWD\..\..\build\debug\.obj
##    Debug:MOC_DIR = $$PWD\..\..\build\debug\.moc
##    Debug:RCC_DIR = $$PWD\..\..\build\debug\.rcc
##    Debug:UI_DIR = $$PWD\..\..\build\debug\.ui

#    RESOURCES += \
#        ..\..\res\resources.qrc

#    CONFIG(debug, debug|release) {
#        LIBS += \
#            -L$$PWD\..\..\build\debug\libs\ -lxmlbuilder \
#            -L$$PWD\..\..\build\debug\libs\ -lstringxmlbuilder \
#            -L$$PWD\..\..\build\debug\libs\ -lbasictable

##        INCLUDEPATH += $$PWD\..\..\build\debug\libs
##        DEPENDPATH += $$PWD\..\..\build\debug\libs
#    }

#    CONFIG(release, debug|release) {
#        LIBS += \
#            -L$$PWD\..\..\build\release\libs\ -lxmlbuilder \
#            -L$$PWD\..\..\build\release\libs\ -lstringxmlbuilder \
#            -L$$PWD\..\..\build\release\libs\ -lbasictable

##        INCLUDEPATH += $$PWD\..\..\build\release\libs
##        DEPENDPATH += $$PWD\..\..\build\release\libs
#    }
#}

unix {
#    Release:DESTDIR = $$PWD/../../build/release/libs
#    Release:OBJECTS_DIR = $$PWD/../../build/release/.obj
#    Release:MOC_DIR = $$PWD/../../build/release/.moc
#    Release:RCC_DIR = $$PWD/../../build/release/.rcc
#    Release:UI_DIR = $$PWD/../../build/release/.ui

#    Debug:DESTDIR = $$PWD/../../build/debug/libs
#    Debug:OBJECTS_DIR = $$PWD/../../build/debug/.obj
#    Debug:MOC_DIR = $$PWD/../../build/debug/.moc
#    Debug:RCC_DIR = $$PWD/../../build/debug/.rcc
#    Debug:UI_DIR = $$PWD/../../build/debug/.ui

#    RESOURCES += \
#        ../../res/resources.qrc

    target.path = /usr/lib
    INSTALLS += target

#    CONFIG(debug, debug|release) {
#        LIBS += \
#            -L$$PWD/../../build/debug/libs/ -lxmlbuilder \
#            -L$$PWD/../../build/debug/libs/ -lstringxmlbuilder \
#            -L$$PWD/../../build/debug/libs/ -lbasictable

#        INCLUDEPATH += $$PWD/../../build/debug/libs
#        DEPENDPATH += $$PWD/../../build/debug/libs
#    }

#    CONFIG(release, debug|release) {
#        LIBS += \
#            -L$$PWD/../../build/release/libs/ -lxmlbuilder \
#            -L$$PWD/../../build/release/libs/ -lstringxmlbuilder \
#            -L$$PWD/../../build/release/libs/ -lbasictable

#        INCLUDEPATH += $$PWD/../../build/release/libs
#        DEPENDPATH += $$PWD/../../build/release/libs
#    }
}
