cmake_minimum_required(VERSION 3.14)

project(processxmlbuilder LANGUAGES CXX VERSION "1.0.0")

if(NOT SUPPRESS_MULTIPLE_DEFINITIONS)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/productinfo.cmake)
create_product_info(PRODUCT_INFO)

add_library(processxmlbuilder
  SHARED
    processxmlbuilder_global.h
    processxmlbuilder.cpp
    processxmlbuilder.h
    ${PRODUCT_INFO}
)

target_include_directories(processxmlbuilder
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
)

target_link_libraries(processxmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(processxmlbuilder
  PRIVATE
    xmlbuilder
)

target_compile_definitions(processxmlbuilder PRIVATE PROCESSXMLBUILDER_LIBRARY)
