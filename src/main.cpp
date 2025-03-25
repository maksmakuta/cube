#include "cube/game/cube.hpp"
#include "cube/graphics/gl/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Overlay Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    auto time = glfwGetTime();
    const auto game = std::make_shared<cube::game::Cube>();
    game->init();
    while (!glfwWindowShouldClose(window)) {
        game->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
        const auto now = glfwGetTime();
        game->update(static_cast<float>(now - time));
        time = now;
    }
    game->clear();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
