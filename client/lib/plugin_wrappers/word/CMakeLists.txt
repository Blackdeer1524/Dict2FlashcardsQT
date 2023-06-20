message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(word_plugin_wrapper WordPluginWrapper.cpp)
target_link_libraries(word_plugin_wrapper PUBLIC
        card
        connection
        basic_plugin_wrapper)
target_include_directories(word_plugin_wrapper PUBLIC ./)

add_executable(word_plugin_wrapper_test WordPluginWrapper_tests.cpp)
target_link_libraries(word_plugin_wrapper_test PRIVATE
        GTest::gtest_main
        card
        word_plugin_wrapper)
add_test(word_plugin_wrapper_test word_plugin_wrapper_test)
