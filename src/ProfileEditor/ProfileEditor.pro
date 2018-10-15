#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T16:36:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProfileEditor
TEMPLATE = app

TRANSLATIONS = peditor_ja_JP.ts

win32 {
    RC_FILE = $$PWD/../../res/app_icons/app_pe.rc
}

DESTDIR = $$OUT_PWD/../../
OBJECTS_DIR = $$OUT_PWD/../../pe/.obj
MOC_DIR = $$OUT_PWD/../../pe/.moc
RCC_DIR = $$OUT_PWD/../../pe/.rcc
UI_DIR = $$OUT_PWD/../../pe/.ui

INCLUDEPATH += $$OUT_PWD/../../library/
DEPENDPATH += $$OUT_PWD/../../library/
LIBRARYPATH = $$OUT_PWD/../../library/

HEADERS  += $$PWD/profileeditor.h \
    $$PWD/profiletreewidget.h \
#    $$PWD/pesharedfunction.h \
#    $$PWD/fileoperationsignalbinder.h \
#    $$PWD/profilexmlbuilder.h \
    basedockwidget.h \
    runtasksignalbinder.h \
    multitaskp.h \
    aboutpe.h

SOURCES += $$PWD/profileeditor.cpp \
    $$PWD/profiletreewidget.cpp \
#    $$PWD/pesharedfunction.cpp \
#    $$PWD/fileoperationsignalbinder.cpp \
#    $$PWD/profilexmlbuilder.cpp \
    basedockwidget.cpp \
    runtasksignalbinder.cpp \
    multitaskp.cpp \
    aboutpe.cpp

SOURCES += main.cpp

FORMS    += $$PWD/profileeditor.ui \
    aboutpe.ui

RESOURCES += \
    $$PWD/../../res/resources.qrc

include(graphicsarea/graphicsarea.pri)
include(editarea/editarea.pri)
include(variantarea/variantarea.pri)
include(settingdialog/settingdialog.pri)
include(testarea/testarea.pri)
include(editcore/editcore.pri)
include(undocommand/undocommand.pri)
include(undocommand_ui/undocommand_ui.pri)

LIBS += \
        -L$$LIBRARYPATH -lVariantConverter \
        -L$$LIBRARYPATH -lxmlbuilder \
        -L$$LIBRARYPATH -lstringxmlbuilder \
        -L$$LIBRARYPATH -lsearchxmlbuilder \
        -L$$LIBRARYPATH -lextrafunctionsxmlbuilder \
        -L$$LIBRARYPATH -lprocessxmlbuilder \
        -L$$LIBRARYPATH -lprocessxmllistgenerator \
        -L$$LIBRARYPATH -lprofilexmlbuilder \
        -L$$LIBRARYPATH -lprofilecombobox \
        -L$$LIBRARYPATH -lbasictable \
        -L$$LIBRARYPATH -lFileQueueTable \
        -L$$LIBRARYPATH -lsearchtable \
        -L$$LIBRARYPATH -lstringtable \
        -L$$LIBRARYPATH -lfileinfodialog \
        -L$$LIBRARYPATH -lfilesearchdialog \
        -L$$LIBRARYPATH -lextrafunctionstable \
        -L$$LIBRARYPATH -lexecutor \
        -L$$LIBRARYPATH -lconsolebase
