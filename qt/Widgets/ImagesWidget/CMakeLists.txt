set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5Concurrent REQUIRED)


set(PROJECT_SOURCES
    ImagesWidget.hpp
    ImagesWidget.cpp
    ImagesWidget.ui
)

add_library(images_widget ${PROJECT_SOURCES})

target_include_directories(images_widget PUBLIC ./)

target_link_libraries(images_widget 
Qt5::Widgets
Qt5::Network
Qt5::Concurrent
image_plugin_wrapper
card
downloader
)
