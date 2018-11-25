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

INCLUDEPATH += $$OUT_PWD/../../library/ \
            $$PWD/processlistarea/ \
            $$PWD/undocommand_ui/ \
            $$PWD/undocommand/ \
            $$PWD/editarea/ \
            $$PWD/adapter/

DEPENDPATH += $$OUT_PWD/../../library/
LIBRARYPATH = $$OUT_PWD/../../library/

HEADERS  += editarea/commandtable.h \
    editarea/editortab.h \
    editarea/innerstackedwidget.h \
#    editarea/profilecombobox.h \
    editarea/searchcombobox.h \
    editarea/variantcombobox.h \
    adapter/editoperator.h \
    adapter/listmimedata.h \
#    adapter/xmllistgenerator.h \
    graphicsarea/flowcellwidget.h \
    graphicsarea/flowtable.h \
    processlistarea/profiletreewidget.h \
    settingdialog/settingdialog.h \
    console/consoleedit.h \
    console/consoleview.h \
    undocommand/addcommand.h \
    undocommand/deletecommand.h \
    undocommand/insertcommand.h \
    undocommand/swapcommand.h \
    undocommand_ui/checkeditallowinput.h \
    undocommand_ui/checkeditloopinf.h \
    undocommand_ui/checkeditsearchinput.h \
    undocommand_ui/checkedittimeout.h \
    undocommand_ui/checkonlyschedulernormal.h \
    undocommand_ui/checkonlyschedulerother.h \
    undocommand_ui/checkonlyschedulerplugin.h \
    undocommand_ui/checkonlyschedulersearch.h \
    undocommand_ui/comboinitialsearchselect.h \
    undocommand_ui/combopluginselect.h \
    undocommand_ui/comboprofileselect.h \
    undocommand_ui/editcheckcommand.h \
    undocommand_ui/editcomboboxcommand.h \
    undocommand_ui/editexectable.h \
    undocommand_ui/editlocalvartable.h \
    undocommand_ui/editlooparguments.h \
    undocommand_ui/editloopmax.h \
    undocommand_ui/editlooprecursive.h \
    undocommand_ui/editplugintable.h \
    undocommand_ui/editscomboboxcommand.h \
    undocommand_ui/editsearchoutput.h \
    undocommand_ui/editsearchtextsep.h \
    undocommand_ui/edittabcommand.h \
    undocommand_ui/edittextcommand.h \
    undocommand_ui/edittimeout.h \
    undocommand_ui/exectableswapoperation.h \
    undocommand_ui/plugintableswapoperation.h \
    undocommand_ui/stringfileoutput.h \
    undocommand_ui/stringprojectauthor.h \
    undocommand_ui/stringprojectdescription.h \
    undocommand_ui/stringprojectname.h \
    undocommand_ui/stringprojectversion.h \
    undocommand_ui/stringsearchsep.h \
    undocommand_ui/swapexectable.h \
    undocommand_ui/swapplugintable.h \
    variantarea/varianttable.h \
    aboutpe.h \
    basedockwidget.h \
    multitaskp.h \
    profileeditor.h \
    profilexmlbuilder.h \
    runtasksignalbinder.h \
    editarea/pluginscombobox.h \
    graphicsarea/processflowtable.h

SOURCES += editarea/commandtable.cpp \
    editarea/editortab.cpp \
    editarea/innerstackedwidget.cpp \
#    editarea/profilecombobox.cpp \
    editarea/searchcombobox.cpp \
    editarea/variantcombobox.cpp \
    adapter/editoperator.cpp \
    adapter/listmimedata.cpp \
#    adapter/xmllistgenerator.cpp \
    graphicsarea/flowcellwidget.cpp \
    graphicsarea/flowtable.cpp \
    processlistarea/profiletreewidget.cpp \
    settingdialog/settingdialog.cpp \
    console/consoleedit.cpp \
    console/consoleview.cpp \
    undocommand/addcommand.cpp \
    undocommand/deletecommand.cpp \
    undocommand/insertcommand.cpp \
    undocommand/swapcommand.cpp \
    undocommand_ui/checkeditallowinput.cpp \
    undocommand_ui/checkeditloopinf.cpp \
    undocommand_ui/checkeditsearchinput.cpp \
    undocommand_ui/checkedittimeout.cpp \
    undocommand_ui/checkonlyschedulernormal.cpp \
    undocommand_ui/checkonlyschedulerother.cpp \
    undocommand_ui/checkonlyschedulerplugin.cpp \
    undocommand_ui/checkonlyschedulersearch.cpp \
    undocommand_ui/comboinitialsearchselect.cpp \
    undocommand_ui/combopluginselect.cpp \
    undocommand_ui/comboprofileselect.cpp \
    undocommand_ui/editcheckcommand.cpp \
    undocommand_ui/editcomboboxcommand.cpp \
    undocommand_ui/editexectable.cpp \
    undocommand_ui/editlocalvartable.cpp \
    undocommand_ui/editlooparguments.cpp \
    undocommand_ui/editloopmax.cpp \
    undocommand_ui/editlooprecursive.cpp \
    undocommand_ui/editplugintable.cpp \
    undocommand_ui/editscomboboxcommand.cpp \
    undocommand_ui/editsearchoutput.cpp \
    undocommand_ui/editsearchtextsep.cpp \
    undocommand_ui/edittabcommand.cpp \
    undocommand_ui/edittextcommand.cpp \
    undocommand_ui/edittimeout.cpp \
    undocommand_ui/exectableswapoperation.cpp \
    undocommand_ui/plugintableswapoperation.cpp \
    undocommand_ui/stringfileoutput.cpp \
    undocommand_ui/stringprojectauthor.cpp \
    undocommand_ui/stringprojectdescription.cpp \
    undocommand_ui/stringprojectname.cpp \
    undocommand_ui/stringprojectversion.cpp \
    undocommand_ui/stringsearchsep.cpp \
    undocommand_ui/swapexectable.cpp \
    undocommand_ui/swapplugintable.cpp \
    variantarea/varianttable.cpp \
    aboutpe.cpp \
    basedockwidget.cpp \
    main.cpp \
    multitaskp.cpp \
    profileeditor.cpp \
    profilexmlbuilder.cpp \
    runtasksignalbinder.cpp \
    editarea/pluginscombobox.cpp \
    graphicsarea/processflowtable.cpp

SOURCES += main.cpp

FORMS    += $$PWD/profileeditor.ui \
    aboutpe.ui \
    graphicsarea/flowcellwidget.ui \
    settingdialog/settingdialog.ui

RESOURCES += \
    $$PWD/../../res/resources.qrc

#include(graphicsarea/graphicsarea.pri)
#include(editarea/editarea.pri)
#include(variantarea/variantarea.pri)
#include(settingdialog/settingdialog.pri)
#include(console/console.pri)
#include(adapter/adapter.pri)
#include(undocommand/undocommand.pri)
#include(undocommand_ui/undocommand_ui.pri)

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
