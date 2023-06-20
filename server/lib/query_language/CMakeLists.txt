set(LANG_HEADERS
    include/scaner.hpp
    include/parser.hpp
    include/interpreter.hpp
    include/classes.hpp
    include/exception.hpp
)

set(LANG_SOURCE
    src/scaner.cpp
    src/parser.cpp
    src/interpreter.cpp
    src/classes.cpp
    src/exception.cpp
)

add_library(query_lang ${LANG_SOURCE} ${LANG_HEADERS})
target_include_directories(query_lang PUBLIC ./include)

add_library(querying src/querying.cpp)
target_include_directories(querying PUBLIC ./include)
target_link_libraries(querying query_lang)


option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(query_lang_tests)
endif()