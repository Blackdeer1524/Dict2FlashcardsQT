find_package(nlohmann_json 3.7.0 REQUIRED)

find_package(Boost COMPONENTS filesystem system date_time python REQUIRED)
message("Include dirs of boost: " ${Boost_INCLUDE_DIRS})
message("Libs of boost: " ${Boost_LIBRARIES})

find_package(PythonLibs REQUIRED)
message("Include dirs of Python: " ${PYTHON_INCLUDE_DIRS})
message("Libs of Python: " ${PYTHON_LIBRARIES})

add_subdirectory(lib)

add_executable(server_main main.cpp)
target_link_libraries(server_main
    plugin_server
    plugins_provider
)
target_include_directories(server_main PRIVATE {})
add_subdirectory(test_client)
