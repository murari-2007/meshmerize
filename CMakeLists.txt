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
    src/perception/sensor_filter.cpp
    src/perception/line_position.cpp
    src/control/pid.cpp
    src/simulator/virtual_robot.cpp
    src/simulator/virtual_sensor.cpp
    src/perception/junction_detector.cpp
    src/robot/orientation.cpp
    src/maze/maze_builder.cpp
    src/exploration/junction_decision.cpp
    src/exploration/exploration_state.cpp
    src/exploration/explorer.cpp
    src/perception/end_detector.cpp
    src/planning/bfs.cpp
    src/planning/dijkstra.cpp
    src/planning/path_optimizer.cpp
    src/control/turn_controller.cpp
    src/execution/route_executor.cpp
    src/execution/recovery.cpp
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

add_executable(test_sensors
    tests/test_sensors.cpp
)

target_link_libraries(test_sensors
    PRIVATE
        meshmerize_core
)

target_include_directories(test_sensors
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_sensors
    COMMAND test_sensors
)

add_executable(test_sensor_filter
    tests/test_sensor_filter.cpp
)

target_link_libraries(test_sensor_filter
    PRIVATE
        meshmerize_core
)

target_include_directories(test_sensor_filter
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_sensor_filter
    COMMAND test_sensor_filter
)

add_executable(test_line_position
    tests/test_line_position.cpp
)

target_link_libraries(test_line_position
    PRIVATE
        meshmerize_core
)

target_include_directories(test_line_position
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_line_position
    COMMAND test_line_position
)

add_executable(test_pid
    tests/test_pid.cpp
)

target_link_libraries(test_pid
    PRIVATE
        meshmerize_core
)

target_include_directories(test_pid
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_pid
    COMMAND test_pid
)

add_executable(test_junction
    tests/test_junction.cpp
)

target_link_libraries(test_junction
    PRIVATE
        meshmerize_core
)

target_include_directories(test_junction
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_junction
    COMMAND test_junction
)

add_executable(test_orientation
    tests/test_orientation.cpp
)

target_link_libraries(test_orientation
    PRIVATE
        meshmerize_core
)

target_include_directories(test_orientation
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_orientation
    COMMAND test_orientation
)

add_executable(test_maze_builder
    tests/test_maze_builder.cpp
)

target_link_libraries(test_maze_builder
    PRIVATE
        meshmerize_core
)

target_include_directories(test_maze_builder
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_maze_builder
    COMMAND test_maze_builder
)

add_executable(test_exploration
    tests/test_exploration.cpp
)

target_link_libraries(test_exploration
    PRIVATE
        meshmerize_core
)

target_include_directories(test_exploration
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_exploration
    COMMAND test_exploration
)

add_executable(test_end_detector
    tests/test_end_detector.cpp
)

target_link_libraries(
    test_end_detector
    PRIVATE
    meshmerize_core
)

add_test(
    NAME test_end_detector
    COMMAND test_end_detector
)

add_executable(test_bfs
    tests/test_bfs.cpp
)

target_link_libraries(
    test_bfs
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_bfs
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_bfs
    COMMAND test_bfs
)

add_executable(test_dijkstra
    tests/test_dijkstra.cpp
)

target_link_libraries(
    test_dijkstra
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_dijkstra
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_dijkstra
    COMMAND test_dijkstra
)

add_executable(test_path_optimizer
    tests/test_path_optimizer.cpp
)

target_link_libraries(
    test_path_optimizer
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_path_optimizer
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_path_optimizer
    COMMAND test_path_optimizer
)

add_executable(test_turn_controller
    tests/test_turn_controller.cpp
)

target_link_libraries(
    test_turn_controller
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_turn_controller
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_turn_controller
    COMMAND test_turn_controller
)

add_executable(test_route_executor
    tests/test_route_executor.cpp
)

target_link_libraries(
    test_route_executor
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_route_executor
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_route_executor
    COMMAND test_route_executor
)

add_executable(test_recovery
    tests/test_recovery.cpp
)

target_link_libraries(
    test_recovery
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_recovery
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_recovery
    COMMAND test_recovery
)

add_executable(test_full_simulation
    tests/test_full_simulation.cpp
)

target_link_libraries(
    test_full_simulation
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_full_simulation
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_full_simulation
    COMMAND test_full_simulation
)

add_executable(test_phase21_stress
    tests/test_phase21_stress.cpp
)

target_link_libraries(
    test_phase21_stress
    PRIVATE
    meshmerize_core
)

target_include_directories(
    test_phase21_stress
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME test_phase21_stress
    COMMAND test_phase21_stress
)