#include <SDL3/SDL_timer.h>

#include "cube/Cube.hpp"
#include "glad/glad.h"

int main() {
    auto window = cube::Window("Cube");
    cube::Cube app;

    auto last_time = SDL_GetTicks();
    while(window.isOpen()) {
        const auto now_time = SDL_GetTicks();
        app.onUpdate(static_cast<float>(now_time - last_time) / 1000.f);
        last_time = now_time;

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                window.close();
            }
            app.onEvent(event);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        app.onDraw();
        window.swapBuffers();
    }
    return 0;
}
