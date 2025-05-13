cmake_minimum_required(VERSION 4.0)
project(cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/LodePNG)
add_subdirectory(libs/Polyline2D)

add_executable(cube
        include/cube/core/Constants.hpp
        include/cube/core/ICallbacks.hpp
        include/cube/core/IGenerator.hpp
        include/cube/entities/Player.hpp
        include/cube/generators/GeneratorFlat.hpp
        include/cube/generators/GeneratorHeight.hpp
        include/cube/graphics/renderer/Renderer.hpp
        include/cube/graphics/renderer/Vertex.hpp
        include/cube/graphics/renderer/VoxelRenderer.hpp
        include/cube/graphics/Camera.hpp
        include/cube/graphics/Font.hpp
        include/cube/graphics/Mesh.hpp
        include/cube/graphics/Shader.hpp
        include/cube/graphics/Texture.hpp
        include/cube/models/Block.hpp
        include/cube/utils/Utils.hpp
        include/cube/world/Chunk.hpp
        include/cube/world/World.hpp
        include/cube/Cube.hpp

        src/entities/Player.cpp
        src/generators/GeneratorFlat.cpp
        src/generators/GeneratorHeight.cpp
        src/graphics/renderer/Renderer.cpp
        src/graphics/renderer/VoxelRenderer.cpp
        src/graphics/Camera.cpp
        src/graphics/Font.cpp
        src/graphics/Mesh.cpp
        src/graphics/Shader.cpp
        src/graphics/Texture.cpp
        src/models/Block.cpp
        src/world/Chunk.cpp
        src/world/World.cpp
        src/utils/Utils.cpp
        src/Cube.cpp
        src/main.cpp
        include/cube/utils/FNoise.hpp
        src/utils/FNoise.cpp
        include/cube/utils/ThreadPool.hpp
        src/utils/ThreadPool.cpp
)

target_include_directories(cube PUBLIC include)
target_link_libraries(cube PUBLIC OpenGL::GL glfw glm::glm Freetype::Freetype GLAD LodePNG Polyline2D)
target_compile_definitions(cube PRIVATE CUBE_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
