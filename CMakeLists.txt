cmake_minimum_required(VERSION 4.0)
project(cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(libs/glad)

add_executable(${PROJECT_NAME}

        cube/core/IManager.hpp
        cube/core/IEntity.hpp
        cube/core/IScreen.hpp

        cube/data/World.cpp
        cube/data/World.hpp
        cube/data/Chunk.cpp
        cube/data/Chunk.hpp

        cube/engine/Engine.cpp
        cube/engine/Engine.hpp
        cube/engine/ScreenManager.cpp
        cube/engine/ScreenManager.hpp

        cube/entity/Player.cpp
        cube/entity/Player.hpp

        cube/graphics/Font.cpp
        cube/graphics/Font.hpp
        cube/graphics/Shader.cpp
        cube/graphics/Shader.hpp
        cube/graphics/Renderer2D.cpp
        cube/graphics/Renderer2D.hpp
        cube/graphics/Renderer3D.cpp
        cube/graphics/Renderer3D.hpp
        cube/graphics/Texture.cpp
        cube/graphics/Texture.hpp

        cube/window/Window.cpp
        cube/window/Window.hpp
        cube/window/Event.cpp
        cube/window/Event.hpp

        cube/main.cpp
        cube/graphics/Context.cpp
        cube/graphics/Context.hpp
        cube/graphics/Color.cpp
        cube/graphics/Color.hpp
        cube/ui/screen/MainScreen.cpp
        cube/ui/screen/MainScreen.hpp
        cube/window/Key.cpp
        cube/window/Key.hpp
        cube/utils/LambdaVisitor.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC cube libs)
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        glfw glm OpenGL::GL glad
)
