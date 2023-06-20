cmake_minimum_required(VERSION 3.22)
project(Dict2FlashcardsQT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD_REQUIRED On)

find_package(Boost COMPONENTS program_options REQUIRED)


enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# Logger
add_subdirectory(spdlog)
link_libraries(spdlog::spdlog_header_only)

add_subdirectory(client)
add_subdirectory(server)
target_include_directories(server_main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/argparse/include/argparse)
add_subdirectory(qt)
