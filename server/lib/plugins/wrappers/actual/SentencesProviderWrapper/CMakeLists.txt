add_library(sentences_provider_wrapper SentencesProviderWrapper.cpp)
target_include_directories(sentences_provider_wrapper PUBLIC ./)
target_link_libraries(sentences_provider_wrapper
    base_plugin_wrapper
    sentences_provider_interface
)
