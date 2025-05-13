#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Cube.hpp"

namespace cube {

    class Window {
    public:
        Window();
        ~Window();

        void setCursorMode(bool isVisible) const;
        void draw();

    private:
        GLFWwindow* m_inner = nullptr;
        Cube game;
    };

}

#endif //WINDOW_HPP
