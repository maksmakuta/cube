#ifndef CUBE_WINDOW_HPP
#define CUBE_WINDOW_HPP

#include <string>

struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const std::string &title);

    ~Window();

    void pollEvents() const;

    void swapBuffers() const;

    [[nodiscard]] bool shouldClose() const;

    [[nodiscard]] GLFWwindow *getGLFWwindow() const { return window; }

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

private:
    GLFWwindow *window;
    int width, height;
};


#endif //CUBE_WINDOW_HPP
