cmake_minimum_required(VERSION 3.20)
project(GraphoDro4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(graphodro4
    src/graph.cpp
    src/algorithms.cpp
    src/parsers.cpp
    src/serializers.cpp
    src/generators.cpp
    src/metrics.cpp
)
target_include_directories(graphodro4 PUBLIC include)

add_executable(graph_cli app/main.cpp)
target_link_libraries(graph_cli PRIVATE graphodro4)

add_executable(graph_tests tests/test_main.cpp)
target_link_libraries(graph_tests PRIVATE graphodro4)

enable_testing()
add_test(NAME graph_tests COMMAND graph_tests)