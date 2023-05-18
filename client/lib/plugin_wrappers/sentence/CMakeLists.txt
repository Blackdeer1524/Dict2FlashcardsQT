message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(sentence_plugin_wrapper SentencePluginWrapper.cpp)
target_link_libraries(sentence_plugin_wrapper PUBLIC
        common
        connection
        basic_plugin_wrapper)
target_include_directories(sentence_plugin_wrapper PUBLIC ./)

add_executable(sentence_plugin_wrapper_test SentencePluginWrapper_tests.cpp)
target_link_libraries(sentence_plugin_wrapper_test PRIVATE
        GTest::gtest_main
        sentence_plugin_wrapper)
add_test(sentence_plugin_wrapper_test sentence_plugin_wrapper_test)
