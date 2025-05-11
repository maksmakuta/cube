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
add_subdirectory(libs/thread_pool)

add_executable(cube
        include/cube/core/Constants.hpp
        include/cube/core/ICallbacks.hpp
        include/cube/core/IGenerator.hpp
        include/cube/entities/Player.hpp
        include/cube/graphics/renderer/Renderer.hpp
        include/cube/graphics/renderer/VoxelRenderer.hpp
        include/cube/graphics/Font.hpp
        include/cube/graphics/Shader.hpp
        include/cube/graphics/Texture.hpp
        include/cube/utils/AssetsUtils.hpp
        include/cube/utils/ColorUtils.hpp
        include/cube/world/Chunk.hpp
        include/cube/world/World.hpp
        include/cube/Cube.hpp

        src/entities/Player.cpp
        src/graphics/renderer/Renderer.cpp
        src/graphics/renderer/VoxelRenderer.cpp
        src/graphics/Shader.cpp
        src/graphics/Texture.cpp
        src/utils/AssetsUtils.cpp
        src/utils/ColorUtils.cpp
        src/Cube.cpp
        src/main.cpp
        include/cube/graphics/Camera.hpp
        src/graphics/Camera.cpp
        include/cube/utils/Utils.hpp
        src/graphics/Font.cpp
        src/world/World.cpp
        src/world/Chunk.cpp
        include/cube/models/Block.hpp
        include/cube/generators/GeneratorFlat.hpp
        src/generators/GeneratorFlat.cpp
        include/cube/graphics/renderer/Vertex.hpp
        include/cube/graphics/Mesh.hpp
        src/graphics/Mesh.cpp
)

target_include_directories(cube PUBLIC include)
target_link_libraries(cube PUBLIC OpenGL::GL glfw glm::glm Freetype::Freetype GLAD LodePNG Polyline2D BS_thread_pool)
target_compile_definitions(cube PRIVATE CUBE_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
