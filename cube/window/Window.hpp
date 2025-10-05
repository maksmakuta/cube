#ifndef CUBE_WINDOW_HPP
#define CUBE_WINDOW_HPP

#include <GLFW/glfw3.h>

#include <string>
#include <glm/vec2.hpp>

namespace cube {

    class Window {
    public:
        Window();
        ~Window();

        [[nodiscard]] glm::vec2 getSize() const;
        [[nodiscard]] glm::vec2 getPos() const;
        [[nodiscard]] std::string getTitle() const;

        [[nodiscard]] bool isClosed() const;

        void setSize(const glm::vec2&) const;
        void setPos(const glm::vec2&) const;
        void setTitle(const std::string&) const;

        void update() const;
        void swapBuffers() const;

    private:
        GLFWwindow* m_window;
    };

}

#endif //CUBE_WINDOW_HPP