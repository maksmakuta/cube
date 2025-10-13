#include <iostream>
#include <memory>
#include <ostream>

#include "graphics/Renderer3D.hpp"
#include "graphics/TextureBuilder.hpp"
#include "model/primitive/Camera.hpp"
#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() : m_window(std::make_shared<cube::Window>("Cube")) {
        m_renderer = std::make_unique<cube::Renderer3D>();
        m_window->setCursor(2);
    }

    void run(){
        while(!m_window->isClosed()) {
            update();
            onDraw(*m_renderer);
            m_window->swapBuffers();
            //std::cout << "pos: " << toString(m_camera.position) << ", rotate: " << m_camera.yaw << ", " << m_camera.pitch << std::endl;
        }
    }

protected:

    static std::string toString(const glm::vec3& v) {
        return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
    }

    static std::string toString(const glm::vec2& v) {
        return std::to_string(v.x) + ", " + std::to_string(v.y);
    }

    void onDraw(cube::Renderer3D& r) {
        cube::clear(0xFF303030);
    }

    void update() {
        const auto now = cube::getTime();
        const auto dt = static_cast<float>(now - m_lastTime);
        m_lastTime = now;

        cube::update();
        while (m_window->isNextEvent()) {
            onEvent(m_window->getEvent());
        }

        m_camera.move(m_movement);
        m_camera.update(dt);
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
                if (ke.pressed) {
                    if (ke.key == cube::Key::W) {
                        m_movement |= cube::Forward;
                    }
                    if (ke.key == cube::Key::S) {
                        m_movement |= cube::Backward;
                    }
                    if (ke.key == cube::Key::A) {
                        m_movement |= cube::Left;
                    }
                    if (ke.key == cube::Key::D) {
                        m_movement |= cube::Right;
                    }
                    if (ke.key == cube::Key::Space) {
                        m_movement |= cube::Up;
                    }
                    if (ke.key == cube::Key::Shift) {
                        m_movement |= cube::Down;
                    }
                } else {
                    m_movement = 0;
                }
            },
            [this](const cube::MouseEvent& me) {
                const auto in = glm::vec2{ me.x, me.y };
                const auto delta = in - mouse;
                m_camera.rotate(delta.x, delta.y);
                mouse = in;
            },
            [](const cube::ScrollEvent& se) {},
            [](const cube::InputEvent& ie) {}
        }, e);
    }

private:
    double m_lastTime = 0;
    uint8_t m_movement = 0;
    cube::Camera m_camera;
    glm::vec2 mouse{0}, view{0};
    std::shared_ptr<cube::Window> m_window = nullptr;
    std::unique_ptr<cube::Renderer3D> m_renderer = nullptr;
};

int main(){
    std::make_unique<CubeGame>()->run();
    return 0;
}
