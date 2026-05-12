#include <SDL3/SDL.h>

#include "cube/Cube.hpp"
#include "cube/utils/Log.hpp"
#include "glad/glad.h"

int main(int argc, char** argv) {
    cube::info("Load SDL3");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        cube::error("SDL_Init Failed: {}", SDL_GetError());
        return EXIT_FAILURE;
    }
    cube::info("Set windows attributes");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    cube::info("Create Window");
    SDL_Window* window = SDL_CreateWindow("Cube", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        cube::error("Window Creation Failed: {}", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    cube::info("Create GL Context");
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        cube::error("GL Context Creation Failed: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    cube::info("Load GLAD");
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        cube::error("GLAD Loading Failed");
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetWindowRelativeMouseMode(window,true);

    SDL_GL_SetSwapInterval(1);
    cube::info("Environment Ready. OpenGL 4.6 Context Active.");
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto cube = cube::Cube(argc, argv);
    bool is_running = true;
    auto tick = SDL_GetTicks();

    while (is_running) {
        const auto now = SDL_GetTicks();
        cube.onUpdate(static_cast<float>(now - tick) / 1000.f);
        tick = now;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            cube.onEvent(event);
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                is_running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.onDraw();
        SDL_GL_SwapWindow(window);
    }

    cube::info("Destroy GL Context");
    SDL_GL_DestroyContext(gl_context);

    cube::info("Destroy Window");
    SDL_DestroyWindow(window);

    cube::info("Unload SDL3");
    SDL_Quit();

    return EXIT_SUCCESS;
}
