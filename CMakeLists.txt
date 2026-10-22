cmake_minimum_required(VERSION 3.29.2)
project(DnD_Manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(DnD_Manager
        main.cpp
        Application/CampaignStore.cpp   Application/CampaignStore.h
        Application/MainMenuPage.cpp
        Application/CampaignPage.cpp    Application/CampaignPage.h
        Application/MainWindow.cpp      Application/MainWindow.h
        Application/MainMenuPage.h
)

target_include_directories(DnD_Manager PRIVATE Application)
target_link_libraries(DnD_Manager PRIVATE Qt6::Widgets)