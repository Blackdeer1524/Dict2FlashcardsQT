message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(basic_plugin_wrapper BasicPluginWrapper.cpp)
target_link_libraries(basic_plugin_wrapper PUBLIC common connection)
target_include_directories(basic_plugin_wrapper INTERFACE ./)

add_subdirectory(tests)