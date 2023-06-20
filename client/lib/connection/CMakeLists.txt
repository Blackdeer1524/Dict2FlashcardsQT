message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(connection src/ServerConnection.cpp)

target_include_directories(connection PUBLIC include)