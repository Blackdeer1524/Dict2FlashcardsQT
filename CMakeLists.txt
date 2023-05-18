cmake_minimum_required(VERSION 3.24)
project(Dict2FlashcardsQT)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED On)

find_package(Boost 1.82 COMPONENTS program_options REQUIRED)

enable_testing()
find_package(GTest REQUIRED)

# Logger
add_subdirectory(spdlog)
link_libraries(spdlog::spdlog_header_only)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(test_client)