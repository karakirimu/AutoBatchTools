cmake_minimum_required(VERSION 3.14)

project(AutoBatchEditor LANGUAGES CXX VERSION "1.0.0")
add_compile_definitions(CURRENT_VERSION="${PROJECT_VERSION}")

if(NOT SUPPRESS_MULTIPLE_DEFINITIONS)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/productinfo.cmake)
create_product_info(PRODUCT_INFO)
set_app_icon("../../res/app_icons/app_pe.ico" ICON_RES)

set(XMLRESOLVER
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/xmlresolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/xmlresolver.h
)

set(UNDOCOMMAND
    undocommand/swapcommand.h
    undocommand/swapcommand.cpp
    undocommand/insertcommand.h
    undocommand/insertcommand.cpp
    undocommand/dragdropcommand.h
    undocommand/dragdropcommand.cpp
    undocommand/deletecommand.h
    undocommand/deletecommand.cpp
    undocommand/commandmap.h
    undocommand/addcommand.h
    undocommand/addcommand.cpp
)

set(UNDOCOMMAND_UI
    undocommand_ui/uicommandmap.h
    undocommand_ui/swapplugintable.h
    undocommand_ui/swapplugintable.cpp
    undocommand_ui/swaplocalvarianttree.h
    undocommand_ui/swaplocalvarianttree.cpp
    undocommand_ui/swapglobalvartree.h
    undocommand_ui/swapglobalvartree.cpp
    undocommand_ui/swapexectable.h
    undocommand_ui/swapexectable.cpp
    undocommand_ui/stringsearchsep.h
    undocommand_ui/stringsearchsep.cpp
    undocommand_ui/stringprojectversion.h
    undocommand_ui/stringprojectversion.cpp
    undocommand_ui/stringprojectname.h
    undocommand_ui/stringprojectname.cpp
    undocommand_ui/stringprojectdescription.h
    undocommand_ui/stringprojectdescription.cpp
    undocommand_ui/stringprojectauthor.h
    undocommand_ui/stringprojectauthor.cpp
    undocommand_ui/stringfileoutput.h
    undocommand_ui/stringfileoutput.cpp
    undocommand_ui/stringbasepath.h
    undocommand_ui/stringbasepath.cpp
    undocommand_ui/edittimeout.h
    undocommand_ui/edittimeout.cpp
    undocommand_ui/edittabcommand.h
    undocommand_ui/edittabcommand.cpp
    undocommand_ui/editsearchoutput.h
    undocommand_ui/editsearchoutput.cpp
    undocommand_ui/editsearchfileoutputtype.h
    undocommand_ui/editsearchfileoutputtype.cpp
    undocommand_ui/editscomboboxcommand.h
    undocommand_ui/editscomboboxcommand.cpp
    undocommand_ui/editplugintable.h
    undocommand_ui/editplugintable.cpp
    undocommand_ui/editlooprecursive.h
    undocommand_ui/editlooprecursive.cpp
    undocommand_ui/editloopmax.h
    undocommand_ui/editloopmax.cpp
    undocommand_ui/editlooparguments.h
    undocommand_ui/editlooparguments.cpp
    undocommand_ui/editlocalvartable.h
    undocommand_ui/editlocalvartable.cpp
    undocommand_ui/editlocalvarianttree.h
    undocommand_ui/editlocalvarianttree.cpp
    undocommand_ui/editglobalvartree.h
    undocommand_ui/editglobalvartree.cpp
    undocommand_ui/editexectable.h
    undocommand_ui/editexectable.cpp
    undocommand_ui/editcomboboxcommand.h
    undocommand_ui/editcomboboxcommand.cpp
    undocommand_ui/dragdropplugintable.h
    undocommand_ui/dragdropplugintable.cpp
    undocommand_ui/dragdropexectable.h
    undocommand_ui/dragdropexectable.cpp
    undocommand_ui/comboprofileselect.h
    undocommand_ui/comboprofileselect.cpp
    undocommand_ui/combopluginselect.h
    undocommand_ui/combopluginselect.cpp
    undocommand_ui/checkonlyschedulersearch.h
    undocommand_ui/checkonlyschedulersearch.cpp
    undocommand_ui/checkonlyschedulerplugin.h
    undocommand_ui/checkonlyschedulerplugin.cpp
    undocommand_ui/checkonlyschedulerother.h
    undocommand_ui/checkonlyschedulerother.cpp
    undocommand_ui/checkonlyschedulernormal.h
    undocommand_ui/checkonlyschedulernormal.cpp
    undocommand_ui/checkedittimeout.h
    undocommand_ui/checkedittimeout.cpp
    undocommand_ui/checkeditsearchinput.h
    undocommand_ui/checkeditsearchinput.cpp
    undocommand_ui/checkeditrundetach.h
    undocommand_ui/checkeditrundetach.cpp
    undocommand_ui/checkeditloopinf.h
    undocommand_ui/checkeditloopinf.cpp
    undocommand_ui/checkeditallowinput.h
    undocommand_ui/checkeditallowinput.cpp
)

set(EDITAREA
    editarea/variantcombobox.h
    editarea/variantcombobox.cpp
    editarea/searchcombobox.h
    editarea/searchcombobox.cpp
    editarea/pluginscombobox.h
    editarea/pluginscombobox.cpp
    editarea/plugincommandtable.h
    editarea/plugincommandtable.cpp
    editarea/infostacked.h
    editarea/infostacked.cpp
    editarea/editortab.h
    editarea/editortab.cpp
    editarea/commandtable.h
    editarea/commandtable.cpp
)

set(ADAPTER
    adapter/listmimedata.h
    adapter/listmimedata.cpp
    adapter/editorcachelist.h
    adapter/editorcachelist.cpp
    adapter/editorcacheconverter.h
    adapter/editorcacheconverter.cpp
    adapter/editorcache.h
    adapter/editorcache.cpp
    adapter/editoperator.h
    adapter/editoperator.cpp
)

set(SETTINGDIALOG
    settingdialog/settingdialog.h
    settingdialog/settingdialog.cpp
    settingdialog/settingdialog.ui
)

set(VARIABLEAREA
    variantarea/varianttree.h
    variantarea/varianttree.cpp
)

set(CONSOLE
    console/consoleview.h
    console/consoleview.cpp
    console/consoleedit.h
    console/consoleedit.cpp
)

set(GRAPHICSAREA
    graphicsarea/typeicondelegate.h
    graphicsarea/typeicondelegate.cpp
    graphicsarea/processflowtable.h
    graphicsarea/processflowtable.cpp
)

# Note: InnerFunctions.h has a signal function,
#       so it probably needs to pass to moc.
set(PLUGIN_INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface/InnerFunctions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface/pluginwidget.h
)

add_executable(AutoBatchEditor
    runtasksignalbinder.h
    runtasksignalbinder.cpp
    profileeditor.h
    profileeditor.cpp
    multitaskp.h
    multitaskp.cpp
    basedockwidget.h
    basedockwidget.cpp
    autosaveselectiondialog.h
    autosaveselectiondialog.cpp
    aboutabe.h
    aboutabe.cpp
    settingconstant.h
    main.cpp
    profileeditor.ui
    aboutabe.ui
    autosaveselectiondialog.ui
    ../../res/resources.qrc
    ${XMLRESOLVER}
    ${ADAPTER}
    ${CONSOLE}
    ${EDITAREA}
    ${GRAPHICSAREA}
    ${PLUGIN_INTERFACE}
    ${SETTINGCACHE}
    ${SETTINGDIALOG}
    ${UNDOCOMMAND_UI}
    ${UNDOCOMMAND}
    ${VARIABLEAREA}
    ${PRODUCT_INFO}
    ${ICON_RES}
)

target_include_directories(AutoBatchEditor
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/processlistarea
    ${CMAKE_CURRENT_SOURCE_DIR}/undocommand_ui
    ${CMAKE_CURRENT_SOURCE_DIR}/undocommand
    ${CMAKE_CURRENT_SOURCE_DIR}/editarea
    ${CMAKE_CURRENT_SOURCE_DIR}/adapter
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/qss
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface
)

target_link_libraries(AutoBatchEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AutoBatchEditor
  PRIVATE
    xmlbuilder
    stringxmlbuilder
    searchxmlbuilder
    pluginsxmlbuilder
    processxmlbuilder
    profilexmlbuilder
    profilecombobox
    basictable
    filesearchloader
    filequeuetable
    searchtable
    stringtable
    fileinfodialog
    filesearchdialog
    pluginstree
    executor
    consolebase
)
