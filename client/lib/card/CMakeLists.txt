message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(card Card.cpp)
target_include_directories(card PUBLIC ./)

add_executable(card_tests Card_tests.cpp)
target_link_libraries(card_tests PRIVATE
        GTest::gtest_main
        card)
add_test(card_tests card_tests)
