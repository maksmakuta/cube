#include <format>
#include <memory>

#include "graphics/WorldRenderer.hpp"
#include "models/Camera.hpp"
#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

class CubeGame {
public:
    CubeGame() :
        m_window(std::make_unique<cube::Window>("Cube"))
    {
        m_renderer = std::make_unique<cube::WorldRenderer>(12345);
        m_window->setCursor(2);
        m_camera.setPosition({0,cube::CHUNK_SIZE.y, 0});
    }

    void run(){
        double time = cube::getTime();
        while(!m_window->isClosed()) {
            const double now = cube::getTime();
            const auto dt = static_cast<float>(now - time);
            const auto fps = 1.f / dt;
            if (fps < 0.01f) {
                m_fps = fps;
            }else {
                m_fps += fps;
                m_fps /= 2;
            }
            m_window->setTitle(std::format("Cube - FPS: {}", m_fps));
            time = now;

            m_camera.update(dt);
            m_renderer->update(m_camera);

            cube::update();
            while (m_window->isNextEvent()) {
                onEvent(m_window->getEvent());
            }

            m_renderer->render(m_camera.getMatrix());

            m_window->swapBuffers();
        }
    }

protected:

    void onEvent(const cube::Event& e) {
        std::visit(cube::LambdaVisitor{
            [this](const cube::ResizeEvent& re) {
                m_renderer->resize({re.width, re.height}, m_camera.getFOV());
            },
            [this](const cube::KeyEvent& ke) {
                if (ke.pressed && ke.key == cube::Key::Escape) {
                    m_window->close();
                }
            },
            [](const cube::MouseEvent&) {},
            [](const cube::ScrollEvent&) {},
            [](const cube::InputEvent&) {}
        }, e);
        m_camera.onEvent(e);
    }

private:
    float m_fps = 0.f;
    cube::Camera m_camera{};
    std::unique_ptr<cube::WorldRenderer> m_renderer = nullptr;
    std::unique_ptr<cube::Window> m_window = nullptr;
};

int main(){
    std::make_unique<CubeGame>()->run();
    return 0;
}