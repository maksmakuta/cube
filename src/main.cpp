#include <print>
#include <glm/vec3.hpp>
#include <SDL3/SDL.h>

#include "cube/graphics/Camera.hpp"
#include "cube/window/Window.hpp"
#include "glad/glad.h"

int main() {
    cube::Window window("Cube");

    cube::Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
    window.setRelativeMouseMode(true);

    uint64_t lastTime = SDL_GetTicks();
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
        }

        glClearColor(0.7f, 0.7f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.swapBuffers();
        const auto dir = camera.getDirection();
        std::println("Position: {:3.2f},{:3.2f},{:3.2f} | {:3.2f},{:3.2f}", newPos.x, newPos.y, newPos.z,dir.x, dir.y);
    }
    return 0;
}
