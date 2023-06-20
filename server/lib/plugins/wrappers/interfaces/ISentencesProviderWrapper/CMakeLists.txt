add_library(sentences_provider_interface INTERFACE)
target_include_directories(sentences_provider_interface INTERFACE .)
target_link_libraries(sentences_provider_interface
    INTERFACE
    plugin_wrapper_interface
)
