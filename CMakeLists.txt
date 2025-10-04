cmake_minimum_required(VERSION 4.0)
project(cube)

add_executable(${PROJECT_NAME}

        cube/core/IEngine.hpp
        cube/core/IObject.hpp
        cube/core/IManager.hpp
        cube/core/IEntity.hpp
        cube/core/IRenderer.hpp

        cube/data/World.cpp
        cube/data/World.hpp
        cube/data/Chunk.cpp
        cube/data/Chunk.hpp

        cube/engine/Engine.cpp
        cube/engine/Engine.hpp

        cube/graphics/Shader.cpp
        cube/graphics/Shader.hpp
        cube/graphics/Renderer2D.cpp
        cube/graphics/Renderer2D.hpp
        cube/graphics/Renderer3D.cpp
        cube/graphics/Renderer3D.hpp
        cube/graphics/Texture.cpp
        cube/graphics/Texture.hpp
        cube/graphics/Window.cpp
        cube/graphics/Window.hpp

        cube/models/Player.cpp
        cube/models/Player.hpp
        cube/models/Camera.cpp
        cube/models/Camera.hpp

        cube/Cube.cpp
        cube/Cube.hpp

        main.cpp
)