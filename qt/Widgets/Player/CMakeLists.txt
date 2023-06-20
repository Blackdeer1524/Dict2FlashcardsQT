set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
FIND_PACKAGE( Qt5MultimediaWidgets REQUIRED )

SET(QT_USE_QTMULTIMEDIA TRUE)
SET(QT_USE_QTMULTIMEDIAWIDGETS TRUE)


set(PROJECT_SOURCES
    Player.hpp
    Player.cpp
    Player.ui
)

add_library(player ${PROJECT_SOURCES})

target_include_directories(player PUBLIC ./)

target_link_libraries(player
Qt5::Widgets
Qt5::Multimedia
card
)

QT5_USE_MODULES (player Multimedia MultimediaWidgets)
