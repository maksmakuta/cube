#include "glad/gl.h"
#include "Window.hpp"

#include <iostream>
#include <ostream>

namespace cube {

    void error(const std::string& message) {
        std::cerr << message << std::endl;
        std::exit(EXIT_FAILURE);
    }

    void glfwFramebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
        if (const auto w = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
            w->push(ResizeEvent{ .width = width, .height = height });
        }
    }

    Key toKey(const int key) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                return Key::Escape;
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                return Key::Shift;
            case GLFW_KEY_LEFT_CONTROL:
            case GLFW_KEY_RIGHT_CONTROL:
                return Key::Control;
            case GLFW_KEY_LEFT_ALT:
            case GLFW_KEY_RIGHT_ALT:
                return Key::Alt;
            case GLFW_KEY_W:
                return Key::W;
            case GLFW_KEY_A:
                return Key::A;
            case GLFW_KEY_S:
                return Key::S;
            case GLFW_KEY_D:
                return Key::D;
            case GLFW_KEY_SPACE:
                return Key::Space;
            case GLFW_MOUSE_BUTTON_LEFT:
                return Key::MouseLeft;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return Key::MouseRight;
            default:
                return Key::Unknown;
        }
    }

    void glfwKeyCallback(GLFWwindow* window, const int key, int , const int action, int) {
        if (const auto w = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
            w->push(KeyEvent{ toKey(key), action != GLFW_RELEASE });
        }
    }

    void glfwCursorPosCallback(GLFWwindow* window, const double xpos, const double ypos) {
        if (const auto w = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
            w->push(MouseEvent{ static_cast<float>(xpos),static_cast<float>(ypos) });
        }
    }

    void glfwScrollCallback(GLFWwindow* window, const double xoffset, const double yoffset) {
        if (const auto w = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
            w->push(ScrollEvent{ static_cast<float>(xoffset),static_cast<float>(yoffset) });
        }
    }

    void glfwCharCallback(GLFWwindow* window, unsigned int codepoint) {
        if (const auto w = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
            w->push(InputEvent{ codepoint });
        }
    }

    Window::Window() : m_window(nullptr){
        if (!glfwInit()) {
            error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
        glfwSetWindowUserPointer(m_window, this);

        glfwSetFramebufferSizeCallback(m_window, glfwFramebufferSizeCallback);
        glfwSetKeyCallback(m_window, glfwKeyCallback);
        glfwSetCursorPosCallback(m_window, glfwCursorPosCallback);
        glfwSetScrollCallback(m_window, glfwScrollCallback);
        glfwSetCharCallback(m_window, glfwCharCallback);
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

    bool Window::isNextEvent() const{
        return !m_events.empty();
    }

    Event Window::getEvent(){
        const auto e = m_events.front();
        m_events.pop();
        return e;
    }

    void Window::push(const Event& e) {
        m_events.push(e);
    }

}
