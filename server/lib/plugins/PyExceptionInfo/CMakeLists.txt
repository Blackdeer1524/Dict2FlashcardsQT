message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(py_exception_info PyExceptionInfo.cpp)
target_include_directories(py_exception_info
    PUBLIC
    ./
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(py_exception_info
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
