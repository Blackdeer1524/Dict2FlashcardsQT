add_library(images_provider_interface INTERFACE)
target_include_directories(images_provider_interface INTERFACE .)
target_link_libraries(images_provider_interface
    INTERFACE
    plugin_wrapper_interface
    media
)

