add_library(format_processor_interface INTERFACE)
target_include_directories(format_processor_interface INTERFACE .)
target_link_libraries(format_processor_interface
    INTERFACE
    plugin_wrapper_interface
)
