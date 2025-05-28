cmake_minimum_required(VERSION 4.0)
project(cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build Noise Tool" FORCE) #example if don't need the graph tool

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/LodePNG)
add_subdirectory(libs/Polyline2D)
add_subdirectory(libs/FastNoise2)

add_executable(cube

        include/cube/core/IGenerator.hpp
        include/cube/core/Constants.hpp
        include/cube/core/IWindowController.hpp
        include/cube/entities/Player.hpp
        include/cube/enums/ChunkStage.hpp
        include/cube/enums/Face.hpp
        include/cube/generators/GeneratorFlat.hpp
        include/cube/generators/GeneratorHeight.hpp
        include/cube/graphics/Renderer.hpp
        include/cube/graphics/Vertex.hpp
        include/cube/graphics/Camera.hpp
        include/cube/graphics/Font.hpp
        include/cube/graphics/Mesher.hpp
        include/cube/graphics/VoxelRenderer.hpp
        include/cube/graphics/Shader.hpp
        include/cube/graphics/Texture.hpp
        include/cube/models/Block.hpp
        include/cube/utils/ThreadPool.hpp
        include/cube/utils/Utils.hpp
        include/cube/world/Chunk.hpp
        include/cube/world/World.hpp
        include/cube/Cube.hpp
        include/cube/Window.hpp

        src/entities/Player.cpp
        src/generators/GeneratorFlat.cpp
        src/generators/GeneratorHeight.cpp
        src/graphics/Camera.cpp
        src/graphics/Renderer.cpp
        src/graphics/Font.cpp
        src/graphics/Mesher.cpp
        src/graphics/VoxelRenderer.cpp
        src/graphics/Shader.cpp
        src/graphics/Texture.cpp
        src/utils/ThreadPool.cpp
        src/utils/Utils.cpp
        src/world/Chunk.cpp
        src/world/World.cpp
        src/Cube.cpp
        src/Window.cpp

        src/main.cpp
        src/models/Block.cpp
)

target_include_directories(cube PUBLIC include)
target_link_libraries(cube PUBLIC OpenGL::GL glfw glm::glm Freetype::Freetype GLAD LodePNG Polyline2D FastNoise2)
target_compile_definitions(cube PRIVATE CUBE_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
