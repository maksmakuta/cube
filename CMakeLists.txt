cmake_minimum_required(VERSION 4.2 FATAL_ERROR)
project(cube VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(libs/glad)

add_executable(${PROJECT_NAME}

    src/main.cpp
    include/cube/data/Chunk.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include libs/glad/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 glm::glm OpenGL::GL glad)
