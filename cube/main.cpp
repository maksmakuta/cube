#include <memory>

#include "graphics/Renderer2D.hpp"
#include "graphics/TextureBuilder.hpp"
#include "utils/AssetsPaths.hpp"
#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() : m_window(std::make_shared<cube::Window>("Cube")) {
        m_renderer = std::make_unique<cube::Renderer2D>();
        m_font.load(cube::getFont("Monocraft-Regular.ttf"), 32);
        m_image = cube::TextureBuilder()
        .setFilter(cube::TextureFilter::Nearest)
            .build(cube::getTexture("blocks/9.png"));

    }

    void run(){
        while(!m_window->isClosed()) {
            update();
            onDraw(*m_renderer);
            m_window->swapBuffers();
        }
    }

protected:

    void onDraw(const cube::Renderer2D& r) {
        cube::clear(0xFF203020);
        r.begin();

        r.text(m_font, cube::Color(0xFFFFFFFF));
        r.print("Hello World!", {10,180});

        r.fill(0xFF00FFFF);
        r.rect({20,20}, {120,120});

        r.fill(m_image);
        r.rect({150,20}, {120,120});

        r.stroke(cube::Color(0xFF0000FF), 15.f);
        r.rect({20,200}, {120,120});

        r.end();
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
                m_renderer->resize({re.width,re.height});
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
    cube::Font m_font;
    cube::Texture m_image;
    std::shared_ptr<cube::Window> m_window = nullptr;
    std::unique_ptr<cube::Renderer2D> m_renderer = nullptr;
};

int main(){
    std::make_unique<CubeGame>()->run();
    return 0;
}