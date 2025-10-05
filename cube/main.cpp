#include <iostream>
#include <memory>
#include <ostream>

#include "graphics/Context.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() {
        m_window = std::make_shared<cube::Window>();
    }

    void run() const {
        while(!m_window->isClosed()) {
            clear(cube::Color(0xFFFFFFFF));
            m_window->update();

            while (m_window->isNextEvent()) {
                const auto e = m_window->getEvent();
                std::cout << cube::toString(e) << std::endl;
            }

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
