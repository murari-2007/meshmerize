cmake_minimum_required(VERSION 3.16)

project(meshmerize_robot
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(meshmerize_core
    src/maze/graph.cpp
    src/maze/maze_map.cpp
    src/simulator/virtual_robot.cpp
)

target_include_directories(meshmerize_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(meshmerize
    src/main.cpp
)

target_link_libraries(meshmerize
    PRIVATE
        meshmerize_core
)

target_include_directories(meshmerize
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(test_core
    tests/test_core.cpp
)

target_link_libraries(test_core
    PRIVATE
        meshmerize_core
)

target_include_directories(test_core
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(test_maze
    tests/test_maze.cpp
)

target_link_libraries(test_maze
    PRIVATE
        meshmerize_core
)

target_include_directories(test_maze
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include

)

add_test(
    NAME test_core
    COMMAND test_core
)

add_test(
    NAME test_maze
    COMMAND test_maze
)

add_executable(test_virtual_robot
    tests/test_virtual_robot.cpp
)

target_link_libraries(test_virtual_robot
    PRIVATE
        meshmerize_core
)

target_include_directories(test_virtual_robot
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_virtual_robot
    COMMAND test_virtual_robot
)