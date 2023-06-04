set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)


set(PROJECT_SOURCES
    Downloader.cpp 
    Downloader.hpp 
)

add_library(downloader ${PROJECT_SOURCES})

target_include_directories(downloader PUBLIC ./)

target_link_libraries(downloader
Qt5::Widgets
Qt5::Network
image_plugin_wrapper
card
)
