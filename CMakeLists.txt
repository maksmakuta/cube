cmake_minimum_required(VERSION 3.31)
project(cube)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)

include_directories(include)

add_executable(cube

        include/cube/base/game.hpp
        include/cube/base/generator.hpp
        include/cube/core/block.hpp
        include/cube/core/constants.hpp
        include/cube/core/types.hpp
        include/cube/game/cube.hpp
        include/cube/generators/generator_flat.hpp
        include/cube/graphics/gl/glad.h
        include/cube/graphics/gl/khrplatform.h
        include/cube/graphics/font.hpp
        include/cube/graphics/shader.hpp
        include/cube/graphics/texture.hpp
        include/cube/graphics/vertex.hpp
        include/cube/render/camera.hpp
        include/cube/utils/utils.hpp
        include/cube/world/chunk.hpp
        include/cube/world/world.hpp

        src/game/cube.cpp
        src/generators/generator_flat.cpp
        src/graphics/gl/glad.c
        src/graphics/font.cpp
        src/graphics/shader.cpp
        src/graphics/texture.cpp
        src/render/camera.cpp
        src/utils/utils.cpp
        src/world/chunk.cpp
        src/world/world.cpp
        src/main.cpp
)

target_include_directories(cube PRIVATE
        Freetype::Freetype
        OpenGL::OpenGL
        glm::glm
        PNG::PNG
        glfw
)

target_link_libraries(cube PRIVATE
        Freetype::Freetype
        OpenGL::OpenGL
        glm::glm
        PNG::PNG
        glfw
)
