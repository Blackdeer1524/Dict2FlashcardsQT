add_executable(basic_plugin_wrapper_test
        init_tests.cpp
        get_default_config_tests.cpp
        set_config_tests.cpp)

target_link_libraries(basic_plugin_wrapper_test PRIVATE
        GTest::gtest_main
        basic_plugin_wrapper)

add_test(basic_plugin_wrapper_test basic_plugin_wrapper_test)
