add_library(audios_provider_wrapper AudiosProviderWrapper.cpp)

target_include_directories(audios_provider_wrapper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(audios_provider_wrapper
    base_plugin_wrapper
    audios_provider_interface
)
