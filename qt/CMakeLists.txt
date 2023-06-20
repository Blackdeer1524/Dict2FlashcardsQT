message(${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

message(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(Models)
add_subdirectory(Widgets)
add_subdirectory(Downloader)

add_executable(app
main.cpp
)

target_link_libraries(app
Qt5::Core
Qt5::Widgets
main_window
sentences_widget
audios_widget
images_widget
connection
sentence_plugin_wrapper
image_plugin_wrapper
audio_plugin_wrapper
player
card
)
