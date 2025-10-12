#include <memory>

#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() {
        m_window    = std::make_shared<cube::Window>("Cube");
    }

    void run(){
        while(!m_window->isClosed()) {
            cube::update();
            while (m_window->isNextEvent()) {
                onEvent(m_window->getEvent());
            }
            m_window->swapBuffers();
        }
    }

    void onEvent(const cube::Event& e) const {
        std::visit(cube::LambdaVisitor{
            [](const cube::ResizeEvent& re) {},
            [this](const cube::KeyEvent& ke) {
                if (ke.pressed && ke.key == cube::Key::Escape) {
                    m_window->close();
                }
            },
            [](const cube::MouseEvent& me) {},
            [](const cube::ScrollEvent& se) {},
            [](const cube::InputEvent& ie) {}
        }, e);
    }


private:
    std::shared_ptr<cube::Window> m_window = nullptr;
};

int main(){
    const auto game = std::make_unique<CubeGame>();
    game->run();
    return 0;
}
