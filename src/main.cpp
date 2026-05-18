#include <iostream>
#include <print>

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "cube/Cube.hpp"

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::println(std::cerr,"Cannot init SDL3: {}", SDL_GetError());
    }

    constexpr auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    SDL_Window* window = SDL_CreateWindow("cube", 800,600, flags);
    if (!window) {
        std::println(std::cerr,"Cannot create window: {}", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::println(std::cerr,"Cannot create OpenGL context: {}", SDL_GetError());
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        std::println(std::cerr,"Cannot load OpenGL functions");
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    SDL_SetWindowRelativeMouseMode(window, true);

    bool running = true;
    cube::Cube engine;
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    Uint64 lastTime = SDL_GetTicksNS();

    while (running) {
        const Uint64 currentTime = SDL_GetTicksNS();
        const float deltaTime = static_cast<float>(currentTime - lastTime) / 1000000000.0f;
        lastTime = currentTime;
        engine.onUpdate(deltaTime);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
            engine.onEvent(event);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        engine.onRender();
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
