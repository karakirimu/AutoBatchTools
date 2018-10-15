#-------------------------------------------------
#
# Project created by QtCreator 2017-09-07T19:56:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = aux
TARGET = ExtraPluginInterface

DEFINES += EXTRAPLUGININTERFACE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# set autput directory
DESTDIR = $$OUT_PWD/../../../library/
OBJECTS_DIR = $$OUT_PWD/../../../library/.obj
MOC_DIR = $$OUT_PWD/../../../library/.moc
RCC_DIR = $$OUT_PWD/../../../library/.rcc
UI_DIR = $$OUT_PWD/../../../library/.ui

INCLUDEPATH += $$OUT_PWD/../../../library/
DEPENDPATH += $$OUT_PWD/../../../library/

#SOURCES += \
#    extraplugininterface.cpp

HEADERS += \
#    extraplugininterface_global.h \
    extraplugininterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
