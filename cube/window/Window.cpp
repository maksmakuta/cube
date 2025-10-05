#include "glad/gl.h"
#include "Window.hpp"

#include <iostream>
#include <ostream>


namespace cube {
    void error(const std::string& message) {
        std::cerr << message << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Window::Window() : m_window(nullptr){
        if (!glfwInit()) {
            error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        m_window = glfwCreateWindow(640, 480, "Cube Window", nullptr, nullptr);
        if (!m_window) {
            error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(m_window);
        if (!gladLoadGL(glfwGetProcAddress)) {
            error("Failed to load OpenGL functions");
        }
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    glm::vec2 Window::getSize() const {
        glm::ivec2 size;
        glfwGetWindowSize(m_window, &size.x, &size.y);
        return size;
    }

    glm::vec2 Window::getPos() const {
        glm::ivec2 pos;
        glfwGetWindowPos(m_window, &pos.x, &pos.y);
        return pos;
    }

    std::string Window::getTitle() const {
        return glfwGetWindowTitle(m_window);
    }

    bool Window::isClosed() const {
        return glfwWindowShouldClose(m_window);
    }

    void Window::setSize(const glm::vec2& v) const {
        const auto i = glm::ivec2(v);
        glfwSetWindowSize(m_window, i.x, i.y);
    }

    void Window::setPos(const glm::vec2& v) const {
        const auto i = glm::ivec2(v);
        glfwSetWindowPos(m_window, i.x, i.y);
    }

    void Window::setTitle(const std::string& t) const {
        glfwSetWindowTitle(m_window, t.c_str());
    }

    void Window::update() const {
        glfwPollEvents();
    }

    void Window::swapBuffers() const {
        glfwSwapBuffers(m_window);
    }

}
