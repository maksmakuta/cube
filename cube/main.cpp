#include <memory>

#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() {
        m_window = std::make_shared<cube::Window>();
    }

    void run() const {
        while(!m_window->isClosed()) {
            m_window->update();
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
            m_window->swapBuffers();
        }
    }

private:
    std::shared_ptr<cube::Window> m_window = nullptr;
};

int main(){
    const auto game = std::make_unique<CubeGame>();
    game->run();
    return 0;
}
