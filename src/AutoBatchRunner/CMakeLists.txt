cmake_minimum_required(VERSION 3.14)

project(AutoBatchRunner LANGUAGES CXX VERSION "1.0.0")
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
set_app_icon("../../res/app_icons/app_abr.ico" ICON_RES)

set(TABLE
    ${CMAKE_CURRENT_SOURCE_DIR}/table/profiletable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/table/profiletable.cpp
)

set(TEXTEDIT
    ${CMAKE_CURRENT_SOURCE_DIR}/textedit/consoleview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textedit/consoleview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/textedit/consoleedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textedit/consoleedit.cpp
)

set(DIALOG
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog/aboutabr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog/aboutabr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog/optiondialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog/optiondialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog/aboutabr.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog/optiondialog.ui
)

add_executable(AutoBatchRunner
    basedockwidget.h
    basedockwidget.cpp
    autobatchrunner.h
    autobatchrunner.cpp
    entrytask.h
    entrytask.cpp
    multitask.h
    multitask.cpp
    settingconstant.h
    main.cpp
    autobatchrunner.ui
    ../../res/resources.qrc
    ${TABLE}
    ${TEXTEDIT}
    ${DIALOG}
    ${PRODUCT_INFO}
    ${ICON_RES}
)

target_include_directories(AutoBatchRunner
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/table
    ${CMAKE_CURRENT_SOURCE_DIR}/textedit
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface
)

target_link_libraries(AutoBatchRunner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AutoBatchRunner
  PRIVATE
    variantconverter
    xmlbuilder
    stringxmlbuilder
    searchxmlbuilder
    pluginsxmlbuilder
    processxmlbuilder
    xmlresolver
    profilexmlbuilder
    profilecombobox
    basictable
    filequeuetable
    searchtable
    stringtable
    fileinfodialog
    filesearchdialog
    filesearchloader
    consolebase
    pluginstree
    executor
)
