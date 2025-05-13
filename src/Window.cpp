#include "cube/Window.hpp"

#include <iostream>

#include "glad/gl.h"
#include "cube/core/Constants.hpp"

namespace cube {


    ICallbacks* getCallbacks(GLFWwindow* w) {
        return static_cast<ICallbacks *>(glfwGetWindowUserPointer(w));
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

    Window::Window() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_DEPTH_BITS,24);

        m_inner = glfwCreateWindow(800, 600, TITLE, nullptr, nullptr);
        if (!m_inner) {
            std::cerr << "Failed to create window\n";
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_inner);
        glfwSwapInterval(1);

        if (!gladLoadGL(glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            return;
        }

        glfwSetWindowUserPointer(m_inner, &game);
        game.onCreate();
        game.onResize(800, 600);

        glfwSetKeyCallback(m_inner, onKey);
        glfwSetMouseButtonCallback(m_inner, onButton);
        glfwSetWindowSizeCallback(m_inner, onResize);
        glfwSetScrollCallback(m_inner, onScroll);
        glfwSetCursorPosCallback(m_inner, onCursor);

    }

    Window::~Window() {
        game.onClear();
        glfwDestroyWindow(m_inner);
        glfwTerminate();
    }

    void Window::setCursorMode(const bool isVisible) const {
        glfwSetInputMode(m_inner, GLFW_CURSOR, isVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    void Window::draw() {
        auto last = glfwGetTime();
        while (!glfwWindowShouldClose(m_inner)) {
            const auto now = glfwGetTime();
            game.onUpdate(static_cast<float>(now - last));
            last = now;
            glfwPollEvents();

            game.onDraw();
            glfwSwapBuffers(m_inner);
        }
    }
}
