add_library(server_test_fixture INTERFACE)
target_include_directories(server_test_fixture INTERFACE ./)
target_link_libraries(server_test_fixture INTERFACE test_sender
)

# AUDIO
add_executable(
    audios_provider_test
    audios_provider_test.cpp
)

target_link_libraries(
    audios_provider_test
    test_sender
    media
    GTest::gtest_main
)


# DEFINITIONS
add_executable(
    definitions_provider_test
    definitions_provider_test.cpp
)

target_link_libraries(
    definitions_provider_test
    test_sender
    GTest::gtest_main
)


# Images
add_executable(
    images_provider_test
    images_provider_test.cpp
)

target_link_libraries(
    images_provider_test
    test_sender
    media
    GTest::gtest_main
)

# Sentences
add_executable(
    sentences_provider_test
    sentences_provider_test.cpp
)

target_link_libraries(
    sentences_provider_test
    test_sender
    GTest::gtest_main
)

# FORMAT PROCESSOR
add_executable(
    format_processor_test
    format_processor_test.cpp
)

target_link_libraries(
    format_processor_test
    test_sender
    GTest::gtest_main
)

gtest_discover_tests(audios_provider_test)
gtest_discover_tests(definitions_provider_test)
gtest_discover_tests(images_provider_test)
gtest_discover_tests(sentences_provider_test)
gtest_discover_tests(format_processor_test)
