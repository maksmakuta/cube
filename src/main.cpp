#include "cube/game/cube.hpp"
#include "cube/graphics/gl/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

cube::game::Cube* getGame(GLFWwindow* window) {
    return static_cast<cube::game::Cube*>(glfwGetWindowUserPointer(window));
};

void onResize(GLFWwindow * window, const int width, const int height) {
    if (const auto game = getGame(window); game != nullptr) {
        game->onResize(width, height);
    }
}

void onCursor(GLFWwindow * window, const double x, const double y) {
    if (const auto game = getGame(window); game != nullptr) {
        game->onCursor(static_cast<float>(x), static_cast<float>(y));
    }
}

void onButton(GLFWwindow * window, const int button, const int action, const int mods) {
    if (const auto game = getGame(window); game != nullptr) {
        game->onKey(button, action, mods);
    }
}

void onKey(GLFWwindow *window, const int key, int, const int action, const int mods) {
    if (const auto game = getGame(window); game != nullptr) {
        game->onKey(key, action, mods);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Overlay Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    const auto game = std::make_shared<cube::game::Cube>();

    glfwSetWindowUserPointer(window,game.get());

    glfwSetWindowSizeCallback(window, onResize);
    glfwSetCursorPosCallback(window, onCursor);
    glfwSetMouseButtonCallback(window, onButton);
    glfwSetKeyCallback(window, onKey);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    game->init();

    auto time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        const auto now = glfwGetTime();
        game->update(static_cast<float>(now - time));
        time = now;

        game->draw();
        glfwSwapBuffers(window);
    }
    game->clear();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
