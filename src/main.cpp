#include <SDL3/SDL.h>

#include "cube/utils/Log.hpp"
#include "glad/glad.h"

int main(const int argc, const char **argv) {
    cube::trace("Load SDL3 library");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cube::error("Cannot init SDL3: {}", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    cube::trace("Create SDL Window");
    constexpr auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    SDL_Window *window = SDL_CreateWindow("Cube", 800,600, flags);
    if (!window) {
        cube::error("Cannot create window: {}", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    cube::trace("Create GL context");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        cube::error("Cannot create OpenGL context: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    cube::trace("Load GLAD library");
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        cube::error("Cannot load GLAD library");
        SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    cube::info("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    cube::info("OpenGL vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    cube::info("OpenGL renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    bool is_running = true;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    is_running = false;
                    break;
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    cube::trace("Unload SDL3 library");
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
