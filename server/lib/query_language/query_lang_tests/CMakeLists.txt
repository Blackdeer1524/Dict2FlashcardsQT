add_executable(query_lang_test
    src/test_query_lang.cpp
)
target_link_libraries(query_lang_test
    query_lang
    GTest::gtest_main)

gtest_discover_tests(query_lang_test)
