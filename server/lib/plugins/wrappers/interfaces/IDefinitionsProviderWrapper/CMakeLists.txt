add_library(definitions_provider_interface INTERFACE)
target_include_directories(definitions_provider_interface INTERFACE .)
target_link_libraries(definitions_provider_interface
    INTERFACE
    plugin_wrapper_interface
    media
)
