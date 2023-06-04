set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(PROJECT_SOURCES
    SentencesWidget.hpp
    SentencesWidget.cpp
    SentencesWidget.ui
)

add_library(sentences_widget ${PROJECT_SOURCES})

target_include_directories(sentences_widget PUBLIC ./)

target_link_libraries(sentences_widget 
Qt5::Widgets
sentence_plugin_wrapper
)
