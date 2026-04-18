#include <cube/window/Window.hpp>
#include <print>
#include <SDL3/SDL_init.h>

#include "glad/glad.h"

namespace cube {
    Window::Window(const std::string& name, const glm::ivec2& size) {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        m_window = SDL_CreateWindow(name.c_str(), size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        m_context = SDL_GL_CreateContext(m_window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            std::println("Failed to initializ GLAD");
            return;
        }

        m_is_open = true;
    }

    Window::~Window() {
        if (m_context) SDL_GL_DestroyContext(m_context);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Window::setTitle(const std::string& title) const {
        if (m_window)
            SDL_SetWindowTitle(m_window, title.c_str());
    }

    void Window::setSize(const glm::ivec2& size) const {
        if (m_window)
            SDL_SetWindowSize(m_window, size.x, size.y);
    }

    void Window::setPosition(const glm::ivec2& position) const {
        if (m_window)
            SDL_SetWindowPosition(m_window, position.x, position.y);
    }

    std::string Window::getTitle() const {
        if (m_window)
            return SDL_GetWindowTitle(m_window);
        return "";
    }

    glm::ivec2 Window::getSize() const {
        glm::ivec2 size;
        if (m_window)
            SDL_GetWindowSize(m_window, &size.x, &size.y);
        return size;
    }

    glm::ivec2 Window::getPosition() const {
        glm::ivec2 position;
        if (m_window)
            SDL_GetWindowPosition(m_window, &position.x, &position.y);
        return position;
    }

    void Window::close() {
        m_is_open = false;
    }

    void Window::swapBuffers() const {
        SDL_GL_SwapWindow(m_window);
    }

    bool Window::isOpen() const {
        return m_is_open;
    }

    void Window::setRelativeMouseMode(const bool cond) const {
        if (m_window)
            SDL_SetWindowRelativeMouseMode(m_window,cond);
    }

}
