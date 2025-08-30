#include <iostream>

#include "cube/window/Window.hpp"
#include "glad/gl.h"

int main() {
    try {
        Window window(1280, 720, "Voxel Demo");

        while (!window.shouldClose()) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window.swapBuffers();
            window.pollEvents();
        }
    } catch (const std::exception &e) {
        std::cerr << "Fatal: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
