message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(common Card.cpp)
target_include_directories(common PUBLIC ./)

add_executable(parse_tags_tests parse_tags_tests.cpp)
target_link_libraries(parse_tags_tests PRIVATE
        GTest::gtest_main
        common)
add_test(parse_tags_tests parse_tags_tests)
