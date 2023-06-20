message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(plugins_loader INTERFACE)
target_include_directories(plugins_loader INTERFACE
    ./
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(plugins_loader
    INTERFACE
    base_plugin_wrapper
    py_exception_info
    definitions_provider_wrapper
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
