message(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(word_cards
word_cards.cpp
# word_cards.h 
)
target_include_directories(word_cards PUBLIC ./)

target_link_libraries(word_cards PUBLIC 
Qt5::Core 
Qt5::Widgets
card
)
