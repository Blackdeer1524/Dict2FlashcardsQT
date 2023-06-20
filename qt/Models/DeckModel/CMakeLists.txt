message(${CMAKE_CURRENT_SOURCE_DIR})

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(deck_model 
Deck.cpp  
deck_model.cpp 
# Deck.hpp 
# deck_model.h 
# IDeck.h 
)

target_include_directories(deck_model PUBLIC ./)

target_link_libraries(deck_model PUBLIC
Qt5::Core 
Qt5::Widgets 
word_cards 
card 
word_plugin_wrapper 
)
