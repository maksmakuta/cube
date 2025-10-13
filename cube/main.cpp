#include <memory>

#include "graphics/Renderer3D.hpp"
#include "graphics/TextureBuilder.hpp"
#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() : m_window(std::make_shared<cube::Window>("Cube")) {
        m_renderer = std::make_unique<cube::Renderer3D>();

    }

    void run(){
        while(!m_window->isClosed()) {
            update();
            onDraw(*m_renderer);
            m_window->swapBuffers();
        }
    }

protected:

    void onDraw(cube::Renderer3D& r) {
        cube::clear(0xFFFFFFFF);
    }

    void update() {
        cube::update();
        while (m_window->isNextEvent()) {
            onEvent(m_window->getEvent());
        }
    }

    void onEvent(const cube::Event& e) {
        std::visit(cube::LambdaVisitor{
            [this](const cube::ResizeEvent& re) {
//                m_renderer->resize({re.width,re.height});
                view = {re.width, re.height};
            },
            [this](const cube::KeyEvent& ke) {
                if (ke.pressed && ke.key == cube::Key::Escape) {
                    m_window->close();
                }
            },
            [this](const cube::MouseEvent& me) {
                mouse = glm::vec2{ me.x, me.y };
            },
            [](const cube::ScrollEvent& se) {},
            [](const cube::InputEvent& ie) {}
        }, e);
    }

private:
    glm::vec2 mouse{0}, view{0};
    std::shared_ptr<cube::Window> m_window = nullptr;
    std::unique_ptr<cube::Renderer3D> m_renderer = nullptr;
};

int main(){
    std::make_unique<CubeGame>()->run();
    return 0;
}
