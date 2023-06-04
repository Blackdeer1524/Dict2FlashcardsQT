cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(main_window
mainwindow.cpp 
# mainwindow.h 
mainwindow.ui
)

target_include_directories(main_window PUBLIC ./)

target_link_libraries(main_window PUBLIC
Qt5::Widgets
connection
deck_model
card
sentences_widget
audios_widget
images_widget
word_plugin_wrapper
format_processor_plugin_wrapper
)