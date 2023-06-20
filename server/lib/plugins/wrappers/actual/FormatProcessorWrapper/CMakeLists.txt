add_library(format_processor_wrapper FormatProcessorWrapper.cpp)

target_include_directories(format_processor_wrapper PUBLIC ./)
target_link_libraries(format_processor_wrapper
    base_plugin_wrapper
    format_processor_interface
)

# set(serverside_plugin_wrappers
#     "${serverside_plugin_wrappers} format_processor_wrapper"
#     PARENT_SCOPE)
