#include <memory>

#include "game/ScreenManager.hpp"
#include "ui/screen/MainScreen.hpp"
#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() {
        m_window    = std::make_shared<cube::Window>("Cube");
        m_context   = std::make_shared<cube::Context>();
        m_context->setRenderer(std::make_shared<cube::Renderer2D>());
        m_manager   = std::make_shared<cube::ScreenManager>();
        m_manager->go<cube::MainScreen>();
    }

    void run(){
        while(!m_window->isClosed()) {
            const double now = cube::getTime();
            const auto dt = static_cast<float>(now - m_time);
            m_time = now;

            cube::update();
            while (m_window->isNextEvent()) {
                onEvent(m_window->getEvent());
            }
            m_manager->onTick(dt);

            m_manager->onDraw(*m_context);
            m_window->swapBuffers();
        }
        m_manager->onDeinit();
        m_manager->clear();
    }

    void onEvent(const cube::Event& e) const {
        std::visit(cube::LambdaVisitor{
            [this](const cube::ResizeEvent& re) {
                if (const auto r = m_context->getRenderer2D()) {
                    r->resize(glm::vec2(re.width, re.height));
                }
            },
            [this](const cube::KeyEvent& ke) {
                if (ke.pressed && ke.key == cube::Key::Escape) {
                    m_window->close();
                }
            },
            [](const cube::MouseEvent& me) {},
            [](const cube::ScrollEvent& se) {},
            [](const cube::InputEvent& ie) {}
        }, e);
        m_manager->onEvent(e);
    }


private:
    std::shared_ptr<cube::Window> m_window = nullptr;
    std::shared_ptr<cube::Context> m_context = nullptr;
    std::shared_ptr<cube::ScreenManager> m_manager = nullptr;
    double m_time = 0.f;
};

int main(){
    const auto game = std::make_unique<CubeGame>();
    game->run();
    return 0;
}
