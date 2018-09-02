#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T22:50:29
#
#-------------------------------------------------

QT       += widgets

TARGET = profilecombobox
TEMPLATE = lib

DEFINES += PROFILECOMBOBOX_LIBRARY

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

include($$PWD/../build_library_path.pri)

SOURCES += \
        profilecombobox.cpp

HEADERS += \
        profilecombobox.h \
        profilecombobox_global.h 

LIBS += \
    -L$$DESTDIR -lxmlbuilder \
    -L$$DESTDIR -lprocessxmlbuilder \
    -L$$DESTDIR -lprofilexmlbuilder

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#unix|win32:CONFIG(debug, debug|release) LIBS += \
#            -L$$PWD/../../build/debug/libs/ -lxmlbuilder \
#            -L$$PWD/../../build/debug/libs/ -lprocessxmlbuilder \
#            -L$$PWD/../../build/debug/libs/ -lprofilexmlbuilder \

#unix|win32:CONFIG(debug, debug|release) INCLUDEPATH += $$PWD/../../build/debug/libs
#unix|win32:CONFIG(debug, debug|release) DEPENDPATH += $$PWD/../../build/debug/libs

#unix|win32:CONFIG(release, debug|release) LIBS += \
#            -L$$PWD/../../build/release/libs/ -lxmlbuilder \
#            -L$$PWD/../../build/release/libs/ -lprocessxmlbuilder \
#            -L$$PWD/../../build/release/libs/ -lprofilexmlbuilder \

#unix|win32:CONFIG(release, debug|release) INCLUDEPATH += $$PWD/../../build/release/libs
#unix|win32:CONFIG(release, debug|release) DEPENDPATH += $$PWD/../../build/release/libs

#unix|win32:CONFIG(debug, debug|release) {
#    LIBS += \
#        -L$$PWD/../../build/debug/libs/ -lxmlbuilder \
#        -L$$PWD/../../build/debug/libs/ -lprocessxmlbuilder \
#        -L$$PWD/../../build/debug/libs/ -lprofilexmlbuilder
##    INCLUDEPATH += $$PWD/../../build/debug/libs
##    DEPENDPATH += $$PWD/../../build/debug/libs
#}

#unix|win32:CONFIG(release, debug|release) {
#    LIBS += \
#        -L$$PWD/../../build/release/libs/ -lxmlbuilder \
#        -L$$PWD/../../build/release/libs/ -lprocessxmlbuilder \
#        -L$$PWD/../../build/release/libs/ -lprofilexmlbuilder
##    INCLUDEPATH += $$PWD/../../build/release/libs
##    DEPENDPATH += $$PWD/../../build/release/libs
#}
