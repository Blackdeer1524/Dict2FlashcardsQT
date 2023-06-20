set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(PROJECT_SOURCES
    AudiosWidget.hpp
    AudiosWidget.cpp
    AudiosWidget.ui
)

add_library(audios_widget ${PROJECT_SOURCES})

target_include_directories(audios_widget PUBLIC ./)

target_link_libraries(audios_widget 
Qt5::Widgets
player
card
audio_plugin_wrapper
)
