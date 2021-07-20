cmake_minimum_required(VERSION 3.14)

project(AutoBatchTools LANGUAGES CXX VERSION "1.0.0")

option(SUPPRESS_MULTIPLE_DEFINITIONS
    "Ignore redefinition of options defined in all projects."
    ON
)
if(SUPPRESS_MULTIPLE_DEFINITIONS)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Set build output directory
string(
    REPLACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/build"
    BUILD_DIR
    ${CMAKE_BINARY_DIR}
)
#message(STATUS "BUILD_DIR=${BUILD_DIR}")

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}/lib")
endif()
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}/lib")
endif()
if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
endif()

# Set source directory
set(SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/src)

# Create Language files
find_package(QT NAMES Qt6 Qt5 COMPONENTS LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS LinguistTools REQUIRED)

set(TS_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/translation)
set(TS_FILES
    abt_en.ts
    abt_ja.ts
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_create_translation(QM_FILES ${TS_SOURCE_DIRECTORY} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${TS_SOURCE_DIRECTORY} ${TS_FILES})
endif()

# 0 No dependent library
add_subdirectory(${SOURCE_DIRECTORY}/xmlbuilder)

add_subdirectory(${SOURCE_DIRECTORY}/basictable)

# 1
add_subdirectory(${SOURCE_DIRECTORY}/profilexmlbuilder)
add_subdirectory(${SOURCE_DIRECTORY}/searchxmlbuilder)
add_subdirectory(${SOURCE_DIRECTORY}/stringxmlbuilder)
add_subdirectory(${SOURCE_DIRECTORY}/pluginsxmlbuilder)
add_subdirectory(${SOURCE_DIRECTORY}/processxmlbuilder)

# 2
add_subdirectory(${SOURCE_DIRECTORY}/filesearchloader)
add_subdirectory(${SOURCE_DIRECTORY}/executor)

# 3
add_subdirectory(${SOURCE_DIRECTORY}/profilecombobox)
add_subdirectory(${SOURCE_DIRECTORY}/consolebase)

# 4
add_subdirectory(${SOURCE_DIRECTORY}/fileinfodialog)
add_subdirectory(${SOURCE_DIRECTORY}/filesearchdialog)

# 5
add_subdirectory(${SOURCE_DIRECTORY}/searchtable)
add_subdirectory(${SOURCE_DIRECTORY}/stringtable)
add_subdirectory(${SOURCE_DIRECTORY}/filequeuetable)
add_subdirectory(${SOURCE_DIRECTORY}/pluginstree)

# 6
add_subdirectory(${SOURCE_DIRECTORY}/AutoBatchRunner)
add_subdirectory(${SOURCE_DIRECTORY}/AutoBatchScheduler)
add_subdirectory(${SOURCE_DIRECTORY}/ProfileEditor)
