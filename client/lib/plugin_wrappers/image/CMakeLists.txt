message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(image_plugin_wrapper ImagePluginWrapper.cpp)
target_link_libraries(image_plugin_wrapper PUBLIC
        common
        connection
        basic_plugin_wrapper)
target_include_directories(image_plugin_wrapper PUBLIC ./)

add_executable(image_plugin_wrapper_test ImagePluginWrapper_tests.cpp)
target_link_libraries(image_plugin_wrapper_test PRIVATE
        GTest::gtest_main
        basic_plugin_wrapper
        image_plugin_wrapper)
add_test(image_plugin_wrapper_test image_plugin_wrapper_test)
