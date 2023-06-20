message(${CMAKE_CURRENT_SOURCE_DIR})

add_library(response_generators ResponseGenerators.cpp)
target_include_directories(response_generators
    PUBLIC
    ./
)
target_link_libraries(response_generators
    querying
    # query_lang
    plugins_bundle
    plugins_provider
)
