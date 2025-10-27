#ifndef CUBE_WINDOW_HPP
#define CUBE_WINDOW_HPP

#include <GLFW/glfw3.h>

#include <string>
#include <queue>
#include <glm/vec2.hpp>

#include "Event.hpp"

namespace cube {

    class Window {
    public:
        explicit Window(const std::string& title, const glm::ivec2& size = {640,480}, int vsync = 0);
        ~Window();

        [[nodiscard]] glm::vec2 getSize() const;
        [[nodiscard]] glm::vec2 getPos() const;
        [[nodiscard]] std::string getTitle() const;
        [[nodiscard]] Event getEvent();

        [[nodiscard]] bool isClosed() const;
        [[nodiscard]] bool isNextEvent() const;

        void setSize(const glm::vec2&) const;
        void setPos(const glm::vec2&) const;
        void setTitle(const std::string&) const;

        void setCursor(int mode) const;

        void swapBuffers() const;
        void push(const Event&);
        void close() const;

    private:
        GLFWwindow* m_window;
        std::queue<Event> m_events;
    };

    void update();
    double getTime();

}

#endif //CUBE_WINDOW_HPP