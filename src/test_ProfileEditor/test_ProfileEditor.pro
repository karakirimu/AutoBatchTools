QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_testvarianttree.cpp \
    ../ProfileEditor/adapter/editoperator.cpp \
    ../ProfileEditor/adapter/listmimedata.cpp \
    ../ProfileEditor/console/consoleedit.cpp \
    ../ProfileEditor/console/consoleview.cpp \
    ../ProfileEditor/editarea/commandtable.cpp \
    ../ProfileEditor/editarea/editortab.cpp \
    ../ProfileEditor/editarea/infostacked.cpp \
    ../ProfileEditor/editarea/pluginscombobox.cpp \
    ../ProfileEditor/editarea/searchcombobox.cpp \
    ../ProfileEditor/editarea/variantcombobox.cpp \
    ../ProfileEditor/graphicsarea/processflowtable.cpp \
    ../ProfileEditor/settingdialog/settingdialog.cpp \
    ../ProfileEditor/undocommand/addcommand.cpp \
    ../ProfileEditor/undocommand/deletecommand.cpp \
    ../ProfileEditor/undocommand/insertcommand.cpp \
    ../ProfileEditor/undocommand/swapcommand.cpp \
    ../ProfileEditor/undocommand_ui/checkeditallowinput.cpp \
    ../ProfileEditor/undocommand_ui/checkeditloopinf.cpp \
    ../ProfileEditor/undocommand_ui/checkeditsearchinput.cpp \
    ../ProfileEditor/undocommand_ui/checkedittimeout.cpp \
    ../ProfileEditor/undocommand_ui/checkonlyschedulernormal.cpp \
    ../ProfileEditor/undocommand_ui/checkonlyschedulerother.cpp \
    ../ProfileEditor/undocommand_ui/checkonlyschedulerplugin.cpp \
    ../ProfileEditor/undocommand_ui/checkonlyschedulersearch.cpp \
    ../ProfileEditor/undocommand_ui/combopluginselect.cpp \
    ../ProfileEditor/undocommand_ui/comboprofileselect.cpp \
    ../ProfileEditor/undocommand_ui/editcomboboxcommand.cpp \
    ../ProfileEditor/undocommand_ui/editexectable.cpp \
    ../ProfileEditor/undocommand_ui/editlocalvartable.cpp \
    ../ProfileEditor/undocommand_ui/editlooparguments.cpp \
    ../ProfileEditor/undocommand_ui/editloopmax.cpp \
    ../ProfileEditor/undocommand_ui/editlooprecursive.cpp \
    ../ProfileEditor/undocommand_ui/editplugintable.cpp \
    ../ProfileEditor/undocommand_ui/editscomboboxcommand.cpp \
    ../ProfileEditor/undocommand_ui/editsearchoutput.cpp \
    ../ProfileEditor/undocommand_ui/edittabcommand.cpp \
    ../ProfileEditor/undocommand_ui/edittimeout.cpp \
    ../ProfileEditor/undocommand_ui/stringfileoutput.cpp \
    ../ProfileEditor/undocommand_ui/stringprojectauthor.cpp \
    ../ProfileEditor/undocommand_ui/stringprojectdescription.cpp \
    ../ProfileEditor/undocommand_ui/stringprojectname.cpp \
    ../ProfileEditor/undocommand_ui/stringprojectversion.cpp \
    ../ProfileEditor/undocommand_ui/stringsearchsep.cpp \
    ../ProfileEditor/undocommand_ui/swapexectable.cpp \
    ../ProfileEditor/undocommand_ui/swapplugintable.cpp \
    ../ProfileEditor/variantarea/varianttable.cpp \
    ../ProfileEditor/variantarea/varianttree.cpp \
    ../ProfileEditor/aboutpe.cpp \
    ../ProfileEditor/basedockwidget.cpp \
    ../ProfileEditor/multitaskp.cpp \
    ../ProfileEditor/profileeditor.cpp \
    ../ProfileEditor/profilexmlbuilder.cpp \
    ../ProfileEditor/runtasksignalbinder.cpp

SUBDIRS += \
    ../ProfileEditor/ProfileEditor.pro

FORMS += \
    ../ProfileEditor/settingdialog/settingdialog.ui \
    ../ProfileEditor/aboutpe.ui \
    ../ProfileEditor/profileeditor.ui

HEADERS += \
    ../ProfileEditor/adapter/editoperator.h \
    ../ProfileEditor/adapter/listmimedata.h \
    ../ProfileEditor/console/consoleedit.h \
    ../ProfileEditor/console/consoleview.h \
    ../ProfileEditor/editarea/commandtable.h \
    ../ProfileEditor/editarea/editortab.h \
    ../ProfileEditor/editarea/infostacked.h \
    ../ProfileEditor/editarea/pluginscombobox.h \
    ../ProfileEditor/editarea/searchcombobox.h \
    ../ProfileEditor/editarea/variantcombobox.h \
    ../ProfileEditor/graphicsarea/processflowtable.h \
    ../ProfileEditor/settingdialog/settingdialog.h \
    ../ProfileEditor/undocommand/addcommand.h \
    ../ProfileEditor/undocommand/deletecommand.h \
    ../ProfileEditor/undocommand/insertcommand.h \
    ../ProfileEditor/undocommand/swapcommand.h \
    ../ProfileEditor/undocommand_ui/checkeditallowinput.h \
    ../ProfileEditor/undocommand_ui/checkeditloopinf.h \
    ../ProfileEditor/undocommand_ui/checkeditsearchinput.h \
    ../ProfileEditor/undocommand_ui/checkedittimeout.h \
    ../ProfileEditor/undocommand_ui/checkonlyschedulernormal.h \
    ../ProfileEditor/undocommand_ui/checkonlyschedulerother.h \
    ../ProfileEditor/undocommand_ui/checkonlyschedulerplugin.h \
    ../ProfileEditor/undocommand_ui/checkonlyschedulersearch.h \
    ../ProfileEditor/undocommand_ui/combopluginselect.h \
    ../ProfileEditor/undocommand_ui/comboprofileselect.h \
    ../ProfileEditor/undocommand_ui/editcomboboxcommand.h \
    ../ProfileEditor/undocommand_ui/editexectable.h \
    ../ProfileEditor/undocommand_ui/editlocalvartable.h \
    ../ProfileEditor/undocommand_ui/editlooparguments.h \
    ../ProfileEditor/undocommand_ui/editloopmax.h \
    ../ProfileEditor/undocommand_ui/editlooprecursive.h \
    ../ProfileEditor/undocommand_ui/editplugintable.h \
    ../ProfileEditor/undocommand_ui/editscomboboxcommand.h \
    ../ProfileEditor/undocommand_ui/editsearchoutput.h \
    ../ProfileEditor/undocommand_ui/edittabcommand.h \
    ../ProfileEditor/undocommand_ui/edittimeout.h \
    ../ProfileEditor/undocommand_ui/stringfileoutput.h \
    ../ProfileEditor/undocommand_ui/stringprojectauthor.h \
    ../ProfileEditor/undocommand_ui/stringprojectdescription.h \
    ../ProfileEditor/undocommand_ui/stringprojectname.h \
    ../ProfileEditor/undocommand_ui/stringprojectversion.h \
    ../ProfileEditor/undocommand_ui/stringsearchsep.h \
    ../ProfileEditor/undocommand_ui/swapexectable.h \
    ../ProfileEditor/undocommand_ui/swapplugintable.h \
    ../ProfileEditor/variantarea/varianttable.h \
    ../ProfileEditor/variantarea/varianttree.h \
    ../ProfileEditor/aboutpe.h \
    ../ProfileEditor/basedockwidget.h \
    ../ProfileEditor/multitaskp.h \
    ../ProfileEditor/profileeditor.h \
    ../ProfileEditor/profilexmlbuilder.h \
    ../ProfileEditor/runtasksignalbinder.h
