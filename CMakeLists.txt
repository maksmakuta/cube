cmake_minimum_required(VERSION 4.0)
project(cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/stb)
add_subdirectory(libs/FastNoise2)

add_executable(${PROJECT_NAME}

        cube/game/Generator.cpp
        cube/game/Generator.hpp

        cube/graphics/Color.cpp
        cube/graphics/Color.hpp
        cube/graphics/Font.cpp
        cube/graphics/Font.hpp
        cube/graphics/Shader.cpp
        cube/graphics/Shader.hpp
        cube/graphics/Renderer2D.cpp
        cube/graphics/Renderer2D.hpp
        cube/graphics/Texture.cpp
        cube/graphics/Texture.hpp
        cube/graphics/TextureArray.cpp
        cube/graphics/TextureArray.hpp
        cube/graphics/TextureBuilder.cpp
        cube/graphics/TextureBuilder.hpp
        cube/graphics/TextureParams.hpp

        cube/models/Camera.cpp
        cube/models/Camera.hpp
        cube/models/world/Block.cpp
        cube/models/world/Block.hpp
        cube/models/world/Chunk.cpp
        cube/models/world/Chunk.hpp
        cube/models/world/World.cpp
        cube/models/world/World.hpp

        cube/utils/AssetsPaths.hpp
        cube/utils/LambdaVisitor.hpp
        cube/utils/ThreadPool.cpp
        cube/utils/ThreadPool.hpp

        cube/window/Window.cpp
        cube/window/Window.hpp
        cube/window/Event.cpp
        cube/window/Event.hpp
        cube/window/Key.cpp
        cube/window/Key.hpp

        cube/main.cpp

)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_PATH=\"${ASSETS_DIR}\")

target_include_directories(${PROJECT_NAME} PUBLIC cube libs)
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        glfw glm OpenGL::GL glad stb Freetype::Freetype FastNoise
)
