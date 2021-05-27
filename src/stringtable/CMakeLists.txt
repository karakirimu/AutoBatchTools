cmake_minimum_required(VERSION 3.14)

project(stringtable LANGUAGES CXX VERSION "1.0.0")

if(NOT SUPPRESS_MULTIPLE_DEFINITIONS)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/productinfo.cmake)
create_product_info(PRODUCT_INFO)

add_library(stringtable
  SHARED
    stringtable_global.h
    stringtable.cpp
    stringtable.h
    ../../res/resources.qrc
    ${PRODUCT_INFO}
)

target_link_libraries(stringtable PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(stringtable
  PRIVATE
    xmlbuilder
    stringxmlbuilder
    basictable
)

target_compile_definitions(stringtable PRIVATE STRINGTABLE_LIBRARY)
