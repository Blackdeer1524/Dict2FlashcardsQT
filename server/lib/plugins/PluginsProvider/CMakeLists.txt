message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(plugins_provider PluginsProvider.cpp)
target_include_directories(plugins_provider PUBLIC
    ./
)

target_link_libraries(plugins_provider
    plugins_loader
    definitions_provider_wrapper
    images_provider_wrapper
    sentences_provider_wrapper
    audios_provider_wrapper
    format_processor_wrapper
)
