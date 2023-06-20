message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(audio_plugin_wrapper AudioPluginWrapper.cpp)
target_link_libraries(audio_plugin_wrapper PUBLIC
        card
        connection
        basic_plugin_wrapper)
target_include_directories(audio_plugin_wrapper PUBLIC ./)

add_executable(audio_plugin_wrapper_test AudioPluginWrapper_tests.cpp)
target_link_libraries(audio_plugin_wrapper_test PRIVATE
        GTest::gtest_main
        basic_plugin_wrapper
        audio_plugin_wrapper)
add_test(audio_plugin_wrapper_test audio_plugin_wrapper_test)
