#include <memory>

#include "game/ScreenManager.hpp"
#include "ui/screen/MainScreen.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() {
        m_window = std::make_shared<cube::Window>();
        m_context = std::make_shared<cube::Context>();
        m_manager = std::make_unique<cube::ScreenManager>();
        m_manager->push(std::make_shared<cube::MainScreen>());
    }

    void run() const {
        constexpr auto dt = 1.f / 144.f;
        m_manager->onInit();
        while(!m_window->isClosed()) {
            m_window->update();
            m_manager->onTick(dt);

            while (m_window->isNextEvent()) {
                m_manager->onEvent(m_window->getEvent());
            }

            m_manager->onDraw(*m_context);
            m_window->swapBuffers();
        }
        m_manager->onDeinit();
        m_manager->clear();
    }

private:
    std::shared_ptr<cube::Window> m_window = nullptr;
    std::shared_ptr<cube::Context> m_context = nullptr;
    std::unique_ptr<cube::ScreenManager> m_manager = nullptr;
};

int main(){
    const auto game = std::make_unique<CubeGame>();
    game->run();
    return 0;
}
