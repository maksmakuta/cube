#include <SDL3/SDL.h>
#include <glad/glad.h>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    constexpr auto flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    const auto window = SDL_CreateWindow("Cube", 800, 600, flags);
    if (!window) {
        return -1;
    }

    const auto gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_GL_SetSwapInterval(1);
    SDL_SetWindowRelativeMouseMode(window,true);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    float time = 0.0f;
    int frames = 0;

    bool is_running = true;
    auto last_tick = SDL_GetTicks();
    while (is_running) {
        frames++;
        const auto now = SDL_GetTicks();
        const auto dt = static_cast<float>(now - last_tick) / 1000.f;
        last_tick = now;

        time += dt;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                is_running = false;
            }
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);

        if (time > 1.f) {
            const auto fps = static_cast<float>(frames) / time;
            time = 0.f;
            frames = 0;
        }
    }

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
