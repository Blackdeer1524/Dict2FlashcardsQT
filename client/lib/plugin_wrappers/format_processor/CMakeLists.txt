message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(format_processor_plugin_wrapper FormatProcessorPluginWrapper.cpp)
target_link_libraries(format_processor_plugin_wrapper PUBLIC
        common
        connection
        basic_plugin_wrapper)
target_include_directories(format_processor_plugin_wrapper PUBLIC ./)

add_executable(format_processor_plugin_wrapper_test FormatProcessorPluginWrapper_tests.cpp)
target_link_libraries(format_processor_plugin_wrapper_test PRIVATE
        GTest::gtest_main
        format_processor_plugin_wrapper)
add_test(format_processor_plugin_wrapper_test format_processor_plugin_wrapper_test)
