#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>

#include "cube/Cube.hpp"

cube::ICallbacks* getCallbacks(GLFWwindow* w) {
    return static_cast<cube::ICallbacks *>(glfwGetWindowUserPointer(w));
}

void onResize(GLFWwindow* window, const int w, const int h) {
    if (const auto c = getCallbacks(window); c != nullptr) {
        c->onResize(w,h);
    }
}

void onKey(GLFWwindow * window, const int key, int, const int action, const int mods) {
    if (const auto c = getCallbacks(window); c != nullptr) {
        c->onKey(key,action,mods);
    }
}

void onButton(GLFWwindow * window, const int button, const int action, const int mods) {
    if (const auto c = getCallbacks(window); c != nullptr) {
        c->onKey(button,action,mods);
    }
}

void onScroll(GLFWwindow * window, const double xoffset, const double yoffset) {
    if (const auto c = getCallbacks(window); c != nullptr) {
        c->onScroll(static_cast<float>(xoffset),static_cast<float>(yoffset));
    }
}

void onCursor(GLFWwindow * window, const double xpos, const double ypos) {
    if (const auto c = getCallbacks(window); c != nullptr) {
        c->onCursor(static_cast<float>(xpos),static_cast<float>(ypos));
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto window = glfwCreateWindow(800, 600, "Cube", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    cube::Cube game;
    glfwSetWindowUserPointer(window, &game);
    game.onCreate();
    game.onResize(800, 600);

    glfwSetKeyCallback(window, onKey);
    glfwSetMouseButtonCallback(window, onButton);
    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetScrollCallback(window, onScroll);
    glfwSetCursorPosCallback(window, onCursor);

    auto last = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        const auto now = glfwGetTime();
        game.onUpdate(static_cast<float>(now - last));
        last = now;
        glfwPollEvents();

        game.onDraw();
        glfwSwapBuffers(window);
    }
    game.onClear();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
