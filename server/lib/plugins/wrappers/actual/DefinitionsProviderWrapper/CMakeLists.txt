add_library(definitions_provider_wrapper DefinitionsProviderWrapper.cpp)
target_include_directories(definitions_provider_wrapper PUBLIC ./)
target_link_libraries(definitions_provider_wrapper
    base_plugin_wrapper
    definitions_provider_interface
)

# set(serverside_plugin_wrappers
#     "${serverside_plugin_wrappers} definitions_provider_wrapper"
#     PARENT_SCOPE)
