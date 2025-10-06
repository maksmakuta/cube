cmake_minimum_required(VERSION 4.0)
project(cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/stb)

add_executable(${PROJECT_NAME}
        cube/core/IRenderer.hpp
        cube/core/IScreen.hpp

        cube/data/Block.cpp
        cube/data/Block.hpp

        cube/game/ScreenManager.cpp
        cube/game/ScreenManager.hpp

        cube/graphics/Context.cpp
        cube/graphics/Context.hpp
        cube/graphics/Color.cpp
        cube/graphics/Color.hpp
        cube/graphics/Font.cpp
        cube/graphics/Font.hpp
        cube/graphics/Shader.cpp
        cube/graphics/Shader.hpp
        cube/graphics/Texture.cpp
        cube/graphics/Texture.hpp
        cube/graphics/TextureArray.cpp
        cube/graphics/TextureArray.hpp

        cube/ui/screen/MainScreen.cpp
        cube/ui/screen/MainScreen.hpp

        cube/utils/LambdaVisitor.hpp

        cube/window/Window.cpp
        cube/window/Window.hpp
        cube/window/Event.cpp
        cube/window/Event.hpp
        cube/window/Key.cpp
        cube/window/Key.hpp

        cube/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC cube libs)
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        glfw glm OpenGL::GL glad stb
)
