cmake_minimum_required(VERSION 4.2 FATAL_ERROR)
project(cube VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/spng)
add_subdirectory(libs/FastNoise2)

add_executable(${PROJECT_NAME}
    src/cube/data/Chunk.cpp
    src/cube/data/Generator.cpp
    src/cube/data/Mesh.cpp
    src/cube/data/World.cpp
    src/cube/graphics/Camera.cpp
    src/cube/graphics/Renderer.cpp
    src/cube/graphics/Shader.cpp
    src/cube/graphics/Vertex.cpp
    src/cube/utils/ThreadPool.cpp
    src/cube/Cube.cpp

    src/main.cpp
    src/cube/core/BlockID.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include libs/glad/include libs/spng/spng libs/FastNoise2/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 glm::glm OpenGL::GL glad spng FastNoise2)
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
