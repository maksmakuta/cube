cmake_minimum_required(VERSION 4.2)
project(cube VERSION 0.2.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glad)
add_subdirectory(libs/libspng)
add_subdirectory(libs/FastNoise2)

find_package(SDL3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES}
        include/cube/core/BlockData.hpp
        include/cube/data/Chunk.hpp
        include/cube/data/World.hpp
        src/cube/data/World.cpp
        include/cube/data/Generator.hpp
        src/cube/data/Generator.cpp
        include/cube/core/Block.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
        GLAD
        SDL3::SDL3
        glm::glm
        OpenGL::GL
        spng
        FastNoise2
)

target_include_directories(${PROJECT_NAME} PRIVATE
        include
        libs/glad/include
        libs/libspng/spng
        libs/FastNoise2/include
)