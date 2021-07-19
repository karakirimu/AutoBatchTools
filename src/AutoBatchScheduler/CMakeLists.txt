cmake_minimum_required(VERSION 3.14)

project(AutoBatchScheduler LANGUAGES CXX VERSION "1.0.0")
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
set_app_icon("../../res/app_icons/app_abs.ico" ICON_RES)

set(TASKSCHEDULER
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/entryscheduler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/entryscheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/schedulerwait.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/schedulerwait.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/entrytask.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/entrytask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/taskscheduler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/taskscheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/taskschedulerconnector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler/taskschedulerconnector.cpp
)

set(SYSTEMTRAY
    ${CMAKE_CURRENT_SOURCE_DIR}/systemtray/systemtray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/systemtray/systemtray.cpp
)

set(STATUSAREA
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/cellinfowidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/cellinfowidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/statuswidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/statuswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/processshowtable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/processshowtable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/schedulerconsoleview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/schedulerconsoleview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/schedulerconsoleedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/schedulerconsoleedit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/cellinfowidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea/statuswidget.ui
)

set(SETTINGCACHE
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/schedulercache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/schedulercacheconverter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/schedulercachelist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/schedulerxmlconstant.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/startupxmlbuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/schedulercacheconverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/schedulercachelist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache/startupxmlbuilder.cpp
)

add_executable(AutoBatchScheduler
    aboutabs.h
    aboutabs.cpp
    mainscheduler.h
    mainscheduler.cpp
    startuptable.h
    startuptable.cpp
    startupdialog.h
    startupdialog.cpp
    settingconstant.h
    main.cpp
    aboutabs.ui
    mainscheduler.ui
    startupdialog.ui
    ../../res/resources.qrc
    ${TASKSCHEDULER}
    ${SYSTEMTRAY}
    ${STATUSAREA}
    ${SETTINGCACHE}
    ${PRODUCT_INFO}
    ${ICON_RES}
)

target_include_directories(AutoBatchScheduler
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler
    ${CMAKE_CURRENT_SOURCE_DIR}/systemtray
    ${CMAKE_CURRENT_SOURCE_DIR}/statusarea
    ${CMAKE_CURRENT_SOURCE_DIR}/settingcache
)

target_link_libraries(AutoBatchScheduler PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AutoBatchScheduler
  PRIVATE
    variantconverter
    xmlbuilder
    basictable
    profilexmlbuilder
    profilecombobox
    executor
    consolebase
)

if(UNIX)
target_link_libraries(AutoBatchScheduler
  PRIVATE
    stringxmlbuilder
    searchxmlbuilder
    processxmlbuilder
    filesearchloader
    xmlresolver
)
endif()
