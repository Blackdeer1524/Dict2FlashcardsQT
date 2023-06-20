add_library(audios_provider_interface INTERFACE)
target_include_directories(audios_provider_interface INTERFACE .)
target_link_libraries(audios_provider_interface
    INTERFACE
    plugin_wrapper_interface
    media
)
