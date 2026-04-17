#include <iostream>
#include <SDL3/SDL.h>

#include "cube/window/Window.hpp"
#include "glad/glad.h"

int main() {
    cube::Window window("Cube");

    while (window.isOpen()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                window.close();
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.swapBuffers();
    }
    return 0;
}
