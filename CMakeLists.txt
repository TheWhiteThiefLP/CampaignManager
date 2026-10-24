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

add_executable(DnD_Manager
        main.cpp
        src/MainWindow.cpp              src/MainWindow.h
        src/pages/MainMenuPage.cpp      src/pages/MainMenuPage.h      src/ui/QTMainMenu.ui
        src/pages/CampaignPage.cpp      src/pages/CampaignPage.h
        src/dialogs/SettingsDialog.cpp  src/dialogs/SettingsDialog.h  src/ui/SettingsDialog.ui
        src/core/CampaignStore.cpp      src/core/CampaignStore.h
        src/core/Settings.cpp           src/core/Settings.h
)

target_include_directories(DnD_Manager PRIVATE
        src
        src/pages
        src/dialogs
        src/core
)
target_link_libraries(DnD_Manager PRIVATE Qt6::Widgets)