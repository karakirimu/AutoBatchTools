cmake_minimum_required(VERSION 3.14)

project(executor LANGUAGES CXX VERSION "1.0.0")

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

# Note: InnerFunctions.h has a signal function,
#       so it probably needs to pass to moc.
set(PLUGIN_INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface/InnerFunctions.h
)

set(XMLRESOLVER
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/xmlresolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/xmlresolver.h
)

add_library(executor
  SHARED
    executor_global.h
    executor.cpp
    executor.h
    ${PLUGIN_INTERFACE}
    ${XMLRESOLVER}
    ${PRODUCT_INFO}
)

target_include_directories(executor
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugininterface
)

target_link_libraries(executor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(executor
  PRIVATE
    xmlbuilder
    stringxmlbuilder
    processxmlbuilder
    filesearchloader
)

target_compile_definitions(executor PRIVATE EXECUTOR_LIBRARY)
