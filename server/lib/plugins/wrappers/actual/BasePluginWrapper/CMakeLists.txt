add_library(base_plugin_wrapper BasePluginWrapper.cpp)
target_include_directories(base_plugin_wrapper PUBLIC ./)
target_link_libraries(base_plugin_wrapper PUBLIC
    plugin_wrapper_interface
)
