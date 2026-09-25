cmake_minimum_required(VERSION 3.16)

project(meshmerize_robot
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(meshmerize
    src/main.cpp
)

target_include_directories(meshmerize
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)