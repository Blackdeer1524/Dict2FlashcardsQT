message(${CMAKE_CURRENT_SOURCE_DIR})

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(saved_deck_model 
SavedDeckModel.cpp
)

target_include_directories(saved_deck_model PUBLIC ./)

target_link_libraries(saved_deck_model PUBLIC
Qt5::Core 
Qt5::Widgets 
card  
)
