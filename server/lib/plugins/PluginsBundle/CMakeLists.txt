message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(plugins_bundle PluginsBundle.cpp)
target_link_libraries(plugins_bundle
    definitions_provider_interface
    images_provider_interface
    sentences_provider_interface
    audios_provider_interface
    format_processor_interface
)
target_include_directories(plugins_bundle PUBLIC ./)
