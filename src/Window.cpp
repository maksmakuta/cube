#include "cube/Window.hpp"

#include <iostream>

#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "cube/core/Constants.hpp"

namespace cube {
    
    Cube* getCallbacks(GLFWwindow* w) {
        return static_cast<Cube*>(glfwGetWindowUserPointer(w));
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

    void onText(GLFWwindow * window, const unsigned int codepoint) {
        if (const auto c = getCallbacks(window); c != nullptr) {
            c->onText(codepoint);
        }
    }

    GLFWwindow* cast(void* ptr) {
        return static_cast<GLFWwindow *>(ptr);
    }

    void init(const WindowParams p) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p.gl_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p.gl_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, p.is_core ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, p.gl_samples);
        glfwWindowHint(GLFW_DEPTH_BITS,p.gl_depth);
    }

    Window::Window(WindowParams) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }
        m_window = glfwCreateWindow(800, 600, TITLE, nullptr, nullptr);
        if (!m_window) {
            std::cerr << "Failed to create window\n";
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(cast(m_window));
        glfwSwapInterval(1);

        if (!gladLoadGL(glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            return;
        }
    }

    Window::~Window() {
        glfwDestroyWindow(static_cast<GLFWwindow *>(m_window));
        glfwTerminate();

    }

    void Window::run() {
        if (!m_game) {
            std::cout << "No game pointer attached" << std::endl;
            return;
        }

        auto last = glfwGetTime();
        while (!isClose()) {
            const auto now = glfwGetTime();
            m_game->onUpdate(static_cast<float>(now - last));
            last = now;
            glfwPollEvents();

            m_game->onDraw();
            glfwSwapBuffers(cast(m_window));
        }
        m_game->onClear();
    }

    void Window::setCursorVisibility(const bool isVisible) {
        glfwSetInputMode(
            static_cast<GLFWwindow *>(m_window),
            GLFW_CURSOR,
            isVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
        );
    }

    void Window::attach(const std::shared_ptr<Cube>& g) {
        m_game = g;
        if (m_game) {
            m_game->onCreate();
            m_game->onResize(800, 600);
            glfwSetWindowUserPointer(cast(m_window), m_game.get());
            glfwSetKeyCallback(cast(m_window), onKey);
            glfwSetMouseButtonCallback(cast(m_window), onButton);
            glfwSetWindowSizeCallback(cast(m_window), onResize);
            glfwSetScrollCallback(cast(m_window), onScroll);
            glfwSetCursorPosCallback(cast(m_window), onCursor);
            glfwSetCharCallback(cast(m_window), onText);
        }
    }

    bool Window::isClose() const {
        return glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
    }

    void Window::close() {
        glfwSetWindowShouldClose(static_cast<GLFWwindow *>(m_window),true);
    }

}
