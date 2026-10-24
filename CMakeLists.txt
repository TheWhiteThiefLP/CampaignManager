cmake_minimum_required(VERSION 3.29.2)
project(DnD_Manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/src/ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES
        src/*.cpp
        src/*.h
        src/*.ui
)

add_executable(DnD_Manager main.cpp ${SOURCES}
        src/pages/TabManager.cpp
        src/pages/TabManager.h
        src/core/MarkdownParser.cpp
        src/core/MarkdownParser.h
        src/pages/NPCEditor.cpp
        src/pages/NPCEditor.h)

target_include_directories(DnD_Manager PRIVATE
        src
        src/pages
        src/dialogs
        src/core
)
target_link_libraries(DnD_Manager PRIVATE Qt6::Widgets)