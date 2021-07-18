cmake_minimum_required(VERSION 3.14)

project(unittest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DGTEST_LANGUAGE_CXX11)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif()

find_package(Threads REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

if ($ENV{GOOGLETEST_DIR})
    set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    if (NOT "" STREQUAL "")
        message(WARNING "Using googletest src dir specified at Qt Creator wizard")
    endif ()
    set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../googletest")
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

# xmlbuilder
add_library(xmlbuilder
  SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../xmlbuilder/xmlbuilder_global.h
    ${CMAKE_CURRENT_LIST_DIR}/../xmlbuilder/xmlbuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../xmlbuilder/xmlbuilder.h
    ${CMAKE_CURRENT_LIST_DIR}/../xmlbuilder/basexmlbuilder.h
    ${CMAKE_CURRENT_LIST_DIR}/../xmlbuilder/basexmlbuilder.cpp
)
target_link_libraries(xmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(xmlbuilder PRIVATE XMLBUILDER_LIBRARY)
# ---

# searchxmlbuilder
add_library(searchxmlbuilder
  SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../searchxmlbuilder/searchxmlbuilder_global.h
    ${CMAKE_CURRENT_LIST_DIR}/../searchxmlbuilder/searchxmlbuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../searchxmlbuilder/searchxmlbuilder.h
)

target_include_directories(searchxmlbuilder
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
)

target_link_libraries(searchxmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(searchxmlbuilder
  PRIVATE
    xmlbuilder
)
target_compile_definitions(searchxmlbuilder PRIVATE SEARCHXMLBUILDER_LIBRARY)
# ---

# profilexmlbuilder
add_library(profilexmlbuilder
  SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../profilexmlbuilder/profilexmlbuilder_global.h
    ${CMAKE_CURRENT_LIST_DIR}/../profilexmlbuilder/profilexmlbuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../profilexmlbuilder/profilexmlbuilder.h
)

target_link_libraries(profilexmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(profilexmlbuilder
  PRIVATE
    xmlbuilder
)

target_compile_definitions(profilexmlbuilder PRIVATE PROFILEXMLBUILDER_LIBRARY)
# ---

# startupxmlbuilder
set(StartupXmlBuilderTest ${CMAKE_CURRENT_LIST_DIR}/../AutoBatchScheduler)
set(STARTUPXMLBUILDER
    ${StartupXmlBuilderTest}/settingcache/startupxmlbuilder.cpp
    ${StartupXmlBuilderTest}/settingcache/startupxmlbuilder.h
)
# ---

# stringxmlbuilder
add_library(stringxmlbuilder
  SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../stringxmlbuilder/stringxmlbuilder_global.h
    ${CMAKE_CURRENT_LIST_DIR}/../stringxmlbuilder/stringxmlbuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../stringxmlbuilder/stringxmlbuilder.h
)

target_include_directories(stringxmlbuilder
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
)

target_link_libraries(stringxmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(stringxmlbuilder
  PRIVATE
    xmlbuilder
)

target_compile_definitions(stringxmlbuilder PRIVATE STRINGXMLBUILDER_LIBRARY)
# ---

# pluginxmlbuilder
add_library(pluginsxmlbuilder
  SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../pluginsxmlbuilder/pluginsxmlbuilder_global.h
    ${CMAKE_CURRENT_LIST_DIR}/../pluginsxmlbuilder/pluginsxmlbuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../pluginsxmlbuilder/pluginsxmlbuilder.h
)

target_include_directories(pluginsxmlbuilder
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
)

target_link_libraries(pluginsxmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(pluginsxmlbuilder
  PRIVATE
    xmlbuilder
)

target_compile_definitions(pluginsxmlbuilder PRIVATE PLUGINSXMLBUILDER_LIBRARY)
# ---

# processxmlbuilder
add_library(processxmlbuilder
  SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../processxmlbuilder/processxmlbuilder_global.h
    ${CMAKE_CURRENT_LIST_DIR}/../processxmlbuilder/processxmlbuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../processxmlbuilder/processxmlbuilder.h
)

target_include_directories(processxmlbuilder
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
)

target_link_libraries(processxmlbuilder PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(processxmlbuilder
  PRIVATE
    xmlbuilder
    processxmllistgenerator
)

target_compile_definitions(processxmlbuilder PRIVATE PROCESSXMLBUILDER_LIBRARY)

# processxmllistgenerator
set(ProcessXmlListGeneratorTest ${CMAKE_CURRENT_LIST_DIR}/../processxmllistgenerator)
add_library(processxmllistgenerator
  SHARED
    ${ProcessXmlListGeneratorTest}/processxmllistgenerator_global.h
    ${ProcessXmlListGeneratorTest}/processxmllistgenerator.cpp
    ${ProcessXmlListGeneratorTest}/processxmllistgenerator.h
)

target_include_directories(processxmllistgenerator
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/process
)

target_link_libraries(processxmllistgenerator PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(processxmllistgenerator PRIVATE PROCESSXMLLISTGENERATOR_LIBRARY)

## ---

# basictable
set(BasicTableTest ${CMAKE_CURRENT_LIST_DIR}/../basictable)
add_library(basictable
  SHARED
    ${BasicTableTest}/basictable_global.h
    ${BasicTableTest}/basictable.cpp
    ${BasicTableTest}/basictable.h
    ${BasicTableTest}/basetable.cpp
    ${BasicTableTest}/basetable.h
    ${BasicTableTest}/tablemimedata.h
    ${BasicTableTest}/tablemimedata.cpp
)

target_link_libraries(basictable PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_compile_definitions(basictable PRIVATE BASICTABLE_LIBRARY)
# ---

# stringtable
set(StringTableTest ${CMAKE_CURRENT_LIST_DIR}/../stringtable)
add_library(stringtable
  SHARED
    ${StringTableTest}/stringtable_global.h
    ${StringTableTest}/stringtable.cpp
    ${StringTableTest}/stringtable.h
    ../../res/resources.qrc
)

target_link_libraries(stringtable PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(stringtable
  PRIVATE
    xmlbuilder
    stringxmlbuilder
    basictable
)

target_compile_definitions(stringtable PRIVATE STRINGTABLE_LIBRARY)
# ---

set (XmlBuilderTestIncludes
    ${CMAKE_CURRENT_LIST_DIR}/../xmlbuilder
    ${CMAKE_CURRENT_LIST_DIR}/../searchxmlbuilder
    ${CMAKE_CURRENT_LIST_DIR}/../profilexmlbuilder
    ${CMAKE_CURRENT_LIST_DIR}/../stringxmlbuilder
    ${CMAKE_CURRENT_LIST_DIR}/../pluginsxmlbuilder
    ${CMAKE_CURRENT_LIST_DIR}/../processxmlbuilder
    ${ProcessXmlListGeneratorTest}
    ${StartupXmlBuilderTest}
)

set (TableTestIncludes
    ${BasicTableTest}
    ${StringTableTest}
)

include_directories(
    ${GTestIncludes}
#    ${CMAKE_CURRENT_LIST_DIR}/../filesearchloader
#   xmlbuilder.test
    ${XmlBuilderTestIncludes}
    ${TableTestIncludes}
#   ---
)

add_executable(unittest
    main.cpp
    xmlbuilder.test.h
    xmllistgenerator.test.h
    table.test.h
    ${STARTUPXMLBUILDER}
    ${GTestFiles}
)
add_test(NAME unittest COMMAND unittest)

set (XmlBuilderLinkLibraries
    xmlbuilder
    searchxmlbuilder
    profilexmlbuilder
    stringxmlbuilder
    pluginsxmlbuilder
    processxmlbuilder
)

set (TableLinkLibraries
    basictable
)

target_link_libraries(unittest
    PRIVATE
    Threads::Threads # pre defined
    ${XmlBuilderLinkLibraries}
    ${TableLinkLibraries}
    processxmllistgenerator
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
)
