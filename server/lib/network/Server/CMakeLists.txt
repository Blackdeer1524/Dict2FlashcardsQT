message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(plugin_server Server.cpp)
target_link_libraries(plugin_server
    session
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
target_include_directories(plugin_server
    PUBLIC
    ./
    PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)
