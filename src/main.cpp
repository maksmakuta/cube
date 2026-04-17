#include <print>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <SDL3/SDL.h>

#include "cube/graphics/Camera.hpp"
#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Vertex.hpp"
#include "cube/window/Window.hpp"
#include "glad/glad.h"

constexpr std::array vertices = {
    // Front face
    cube::Vertex{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}}, // Red
    cube::Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}, // Green
    cube::Vertex{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // Blue
    cube::Vertex{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // White
    // Back face
    cube::Vertex{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}, // Yellow
    cube::Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}}, // Cyan
    cube::Vertex{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}}, // Magenta
    cube::Vertex{{-0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}}  // Grey
};

const std::array<uint32_t, 36> indices = {
    0, 1, 2, 2, 3, 0, // Front
    1, 5, 6, 6, 2, 1, // Right
    7, 6, 5, 5, 4, 7, // Back
    4, 0, 3, 3, 7, 4, // Left
    4, 5, 1, 1, 0, 4, // Bottom
    3, 2, 6, 6, 7, 3  // Top
};

int main() {
    cube::Window window("Cube");

    cube::Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
    window.setRelativeMouseMode(true);

    GLuint vao, vbo, ebo;

    cube::Shader cubeShader("../assets/shaders/cube.vert", "../assets/shaders/cube.frag");

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    glNamedBufferStorage(vbo, sizeof(cube::Vertex) * vertices.size(), vertices.data(), 0);
    glNamedBufferStorage(ebo, sizeof(uint32_t) * indices.size(), indices.data(), 0);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(cube::Vertex));
    glVertexArrayElementBuffer(vao, ebo);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(cube::Vertex, position));
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(cube::Vertex, color));
    glVertexArrayAttribBinding(vao, 1, 0);

    glEnable(GL_DEPTH_TEST);

    uint64_t lastTime = SDL_GetTicks();
    glm::mat4 projection{1.f};
    projection = glm::perspectiveFov<float>(glm::radians(45.f),window.getSize().x, window.getSize().y, 0.1f, 64.f);
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    while (window.isOpen()) {
        uint64_t currentTime = SDL_GetTicks();
        const auto deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        auto newPos = camera.getPosition();

        constexpr auto CAMERA_SPEED = 5.0f;

        const auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W]) { newPos += camera.getForward() * deltaTime * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_S]) { newPos -= camera.getForward() * deltaTime * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_A]) { newPos -= camera.getRight() * deltaTime * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_D]) { newPos += camera.getRight() * deltaTime * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_SPACE]) { newPos += camera.getUp() * deltaTime * CAMERA_SPEED; }
        if (keys[SDL_SCANCODE_LSHIFT]) { newPos -= camera.getUp() * deltaTime * CAMERA_SPEED; }

        camera.setPosition(newPos);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                window.close();
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                camera.rotate(event.motion.xrel, -event.motion.yrel);
            }
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
                projection = glm::perspectiveFov<float>(glm::radians(45.f),event.window.data1, event.window.data2, 0.1f, 64.f);
            }
        }

        glClearColor(0.7f, 0.7f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeShader.use();

        // Set your MVP matrices
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", camera.getViewMatrix());
        cubeShader.setMat4("model", glm::mat4(1.0f));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        const auto dir = camera.getDirection();
        std::println("Position: {:3.2f},{:3.2f},{:3.2f} | {:3.2f},{:3.2f}", newPos.x, newPos.y, newPos.z,dir.x, dir.y);
    }
    return 0;
}
