#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T16:36:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoBatchEditor
TEMPLATE = app

VERSION = 1.0.0

win32 {
    QMAKE_TARGET_COMPANY = karakirimu
    QMAKE_TARGET_PRODUCT = AutoBatchEditor
    QMAKE_TARGET_DESCRIPTION = Process execution file editor.
    QMAKE_TARGET_COPYRIGHT = Copyright 2016-2021 karakirimu
    RC_ICONS = ../../res/app_icons/app_pe.ico
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
    aboutabe.h \
    adapter/editorcache.h \
    adapter/editorcacheconverter.h \
    adapter/editorcachelist.h \
    autosaveselectiondialog.h \
    editarea/editortab.h \
    editarea/plugincommandtable.h \
    editarea/searchcombobox.h \
    editarea/variantcombobox.h \
    adapter/editoperator.h \
    adapter/listmimedata.h \
    graphicsarea/typeicondelegate.h \
    settingconstant.h \
    settingdialog/settingdialog.h \
    console/consoleedit.h \
    console/consoleview.h \
    undocommand/addcommand.h \
    undocommand/commandmap.h \
    undocommand/deletecommand.h \
    undocommand/dragdropcommand.h \
    undocommand/insertcommand.h \
    undocommand/swapcommand.h \
    undocommand_ui/checkeditallowinput.h \
    undocommand_ui/checkeditloopinf.h \
    undocommand_ui/checkeditrundetach.h \
    undocommand_ui/checkeditsearchinput.h \
    undocommand_ui/checkedittimeout.h \
    undocommand_ui/checkonlyschedulernormal.h \
    undocommand_ui/checkonlyschedulerother.h \
    undocommand_ui/checkonlyschedulerplugin.h \
    undocommand_ui/checkonlyschedulersearch.h \
    undocommand_ui/combopluginselect.h \
    undocommand_ui/comboprofileselect.h \
    undocommand_ui/dragdropexectable.h \
    undocommand_ui/dragdropplugintable.h \
    undocommand_ui/editcomboboxcommand.h \
    undocommand_ui/editexectable.h \
    undocommand_ui/editlocalvartable.h \
    undocommand_ui/editlooparguments.h \
    undocommand_ui/editloopmax.h \
    undocommand_ui/editlooprecursive.h \
    undocommand_ui/editplugintable.h \
    undocommand_ui/editscomboboxcommand.h \
    undocommand_ui/editsearchfileoutputtype.h \
    undocommand_ui/editsearchoutput.h \
    undocommand_ui/edittabcommand.h \
    undocommand_ui/edittimeout.h \
    undocommand_ui/stringbasepath.h \
    undocommand_ui/stringfileoutput.h \
    undocommand_ui/stringprojectauthor.h \
    undocommand_ui/stringprojectdescription.h \
    undocommand_ui/stringprojectname.h \
    undocommand_ui/stringprojectversion.h \
    undocommand_ui/stringsearchsep.h \
    undocommand_ui/swapexectable.h \
    undocommand_ui/swapplugintable.h \
    undocommand_ui/uicommandmap.h \
    basedockwidget.h \
    multitaskp.h \
    profileeditor.h \
    runtasksignalbinder.h \
    editarea/pluginscombobox.h \
    graphicsarea/processflowtable.h \
    editarea/infostacked.h \
    variantarea/varianttree.h \
    undocommand_ui/editlocalvarianttree.h \
    undocommand_ui/swaplocalvarianttree.h \
    undocommand_ui/editglobalvartree.h \
    undocommand_ui/swapglobalvartree.h

SOURCES += editarea/commandtable.cpp \
    aboutabe.cpp \
    adapter/editorcache.cpp \
    adapter/editorcacheconverter.cpp \
    adapter/editorcachelist.cpp \
    autosaveselectiondialog.cpp \
    editarea/editortab.cpp \
    editarea/plugincommandtable.cpp \
    editarea/searchcombobox.cpp \
    editarea/variantcombobox.cpp \
    adapter/editoperator.cpp \
    adapter/listmimedata.cpp \
    graphicsarea/typeicondelegate.cpp \
    settingdialog/settingdialog.cpp \
    console/consoleedit.cpp \
    console/consoleview.cpp \
    undocommand/addcommand.cpp \
    undocommand/deletecommand.cpp \
    undocommand/dragdropcommand.cpp \
    undocommand/insertcommand.cpp \
    undocommand/swapcommand.cpp \
    undocommand_ui/checkeditallowinput.cpp \
    undocommand_ui/checkeditloopinf.cpp \
    undocommand_ui/checkeditrundetach.cpp \
    undocommand_ui/checkeditsearchinput.cpp \
    undocommand_ui/checkedittimeout.cpp \
    undocommand_ui/checkonlyschedulernormal.cpp \
    undocommand_ui/checkonlyschedulerother.cpp \
    undocommand_ui/checkonlyschedulerplugin.cpp \
    undocommand_ui/checkonlyschedulersearch.cpp \
    undocommand_ui/combopluginselect.cpp \
    undocommand_ui/comboprofileselect.cpp \
    undocommand_ui/dragdropexectable.cpp \
    undocommand_ui/dragdropplugintable.cpp \
    undocommand_ui/editcomboboxcommand.cpp \
    undocommand_ui/editexectable.cpp \
    undocommand_ui/editlocalvartable.cpp \
    undocommand_ui/editlooparguments.cpp \
    undocommand_ui/editloopmax.cpp \
    undocommand_ui/editlooprecursive.cpp \
    undocommand_ui/editplugintable.cpp \
    undocommand_ui/editscomboboxcommand.cpp \
    undocommand_ui/editsearchfileoutputtype.cpp \
    undocommand_ui/editsearchoutput.cpp \
    undocommand_ui/edittabcommand.cpp \
    undocommand_ui/edittimeout.cpp \
    undocommand_ui/stringbasepath.cpp \
    undocommand_ui/stringfileoutput.cpp \
    undocommand_ui/stringprojectauthor.cpp \
    undocommand_ui/stringprojectdescription.cpp \
    undocommand_ui/stringprojectname.cpp \
    undocommand_ui/stringprojectversion.cpp \
    undocommand_ui/stringsearchsep.cpp \
    undocommand_ui/swapexectable.cpp \
    undocommand_ui/swapplugintable.cpp \
    basedockwidget.cpp \
    multitaskp.cpp \
    profileeditor.cpp \
    runtasksignalbinder.cpp \
    editarea/pluginscombobox.cpp \
    graphicsarea/processflowtable.cpp \
    editarea/infostacked.cpp \
    variantarea/varianttree.cpp \
    undocommand_ui/editlocalvarianttree.cpp \
    undocommand_ui/swaplocalvarianttree.cpp \
    undocommand_ui/editglobalvartree.cpp \
    undocommand_ui/swapglobalvartree.cpp

SOURCES += main.cpp

FORMS    += $$PWD/profileeditor.ui \
    aboutabe.ui \
    autosaveselectiondialog.ui \
    settingdialog/settingdialog.ui

RESOURCES += \
    $$PWD/../../res/resources.qrc

# direct loading files
include($$PWD/../plugininterface/plugininterface.pri)
include($$PWD/../share/share.pri)

LIBS += \
        -L$$LIBRARYPATH -lvariantconverter \
        -L$$LIBRARYPATH -lxmlbuilder \
        -L$$LIBRARYPATH -lstringxmlbuilder \
        -L$$LIBRARYPATH -lsearchxmlbuilder \
        -L$$LIBRARYPATH -lpluginsxmlbuilder \
        -L$$LIBRARYPATH -lprocessxmlbuilder \
        -L$$LIBRARYPATH -lprocessxmllistgenerator \
        -L$$LIBRARYPATH -lprofilexmlbuilder \
        -L$$LIBRARYPATH -lprofilecombobox \
        -L$$LIBRARYPATH -lbasictable \
        -L$$LIBRARYPATH -lfilesearchloader \
        -L$$LIBRARYPATH -lfilequeuetable \
        -L$$LIBRARYPATH -lsearchtable \
        -L$$LIBRARYPATH -lstringtable \
        -L$$LIBRARYPATH -lfileinfodialog \
        -L$$LIBRARYPATH -lfilesearchdialog \
        -L$$LIBRARYPATH -lpluginstree \
        -L$$LIBRARYPATH -lexecutor \
        -L$$LIBRARYPATH -lconsolebase
