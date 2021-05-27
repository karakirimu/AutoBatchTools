cmake_minimum_required(VERSION 3.14)

project(pluginstree LANGUAGES CXX VERSION "1.0.0")

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

add_library(pluginstree
  SHARED
    pluginstree_global.h
    pluginstree.cpp
    pluginstree.h
    ${PRODUCT_INFO}
)

target_link_libraries(pluginstree PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(pluginstree
  PRIVATE
    filesearchloader
    pluginsxmlbuilder
)

target_compile_definitions(pluginstree PRIVATE PLUGINSTREE_LIBRARY)
