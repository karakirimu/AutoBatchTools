cmake_minimum_required(VERSION 3.14)

project(profilecombobox LANGUAGES CXX VERSION "1.0.0")

if(NOT SUPPRESS_MULTIPLE_DEFINITIONS)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/productinfo.cmake)
create_product_info(PRODUCT_INFO)

add_library(profilecombobox
  SHARED
    profilecombobox_global.h
    profilecombobox.cpp
    profilecombobox.h
    ${PRODUCT_INFO}
)

target_include_directories(profilecombobox
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface
)

target_link_libraries(profilecombobox PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(profilecombobox
  PRIVATE
    xmlbuilder
    processxmlbuilder
    profilexmlbuilder
    processxmllistgenerator
)

target_compile_definitions(profilecombobox PRIVATE PROFILECOMBOBOX_LIBRARY)
