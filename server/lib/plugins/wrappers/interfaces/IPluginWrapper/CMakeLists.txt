add_library(plugin_wrapper_interface INTERFACE)
target_include_directories(plugin_wrapper_interface INTERFACE .)
target_link_libraries(plugin_wrapper_interface
    INTERFACE
    py_exception_info
)
