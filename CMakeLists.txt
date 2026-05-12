cmake_minimum_required(VERSION 4.2 FATAL_ERROR)
project(cube VERSION 0.3.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/spng)
add_subdirectory(libs/FastNoise2)

add_executable(${PROJECT_NAME}
    include/cube/core/Block.hpp
    include/cube/core/ChunkStatus.hpp
    include/cube/data/Chunk.hpp
    include/cube/data/World.hpp
    include/cube/data/Generator.hpp
    include/cube/graphics/Camera.hpp
    include/cube/graphics/Renderer.hpp
    include/cube/graphics/Shader.hpp
    include/cube/graphics/Vertex.hpp
    include/cube/mesh/Mesher.hpp
    include/cube/utils/Log.hpp
    include/cube/utils/SafeQueue.hpp
    include/cube/Cube.hpp

    src/cube/data/Chunk.cpp
    src/cube/data/World.cpp
    src/cube/data/Generator.cpp
    src/cube/graphics/Camera.cpp
    src/cube/graphics/Renderer.cpp
    src/cube/graphics/Shader.cpp
    src/cube/mesh/Mesher.cpp
    src/cube/utils/Log.cpp
    src/cube/Cube.cpp

    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL SDL3::SDL3 glm::glm glad spng FastNoise2
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    libs/glad/include
    libs/libsnpg/spng
    libs/FastNoise2/include
)

set(ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/")

file(TO_CMAKE_PATH "${ASSETS_PATH}" ASSETS_PATH)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/assets_dir.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/assets_dir.hpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")