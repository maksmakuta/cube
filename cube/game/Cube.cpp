#include "Cube.hpp"

#include <iostream>
#include <random>

#include "graphics/Color.hpp"
#include "utils/LambdaVisitor.hpp"

namespace cube {

    int randomSeed() {
        return static_cast<int>(std::mt19937{std::random_device{}()}());
    }

    Cube::Cube() : m_gen(randomSeed()), m_renderer(std::make_unique<ChunkRenderer>()){
        std::cout << "Seed: " << m_gen.getSeed() << std::endl;
    }

    Cube::~Cube() = default;

    void Cube::onDraw() {
        clear(0xFF353535);
        m_renderer->draw(m_camera.getMatrix());
    }

    void Cube::onUpdate(const float dt) {
        m_camera.update(dt);
    }

    void Cube::onEvent(const Event& e) {
        m_camera.onEvent(e);
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re) {
                m_renderer->resize({re.width, re.height},m_camera.getFOV());
            },
            [](const MouseEvent&) {},
            [](const KeyEvent&) {},
            [](const ScrollEvent&) {},
            [](const InputEvent&) {},
        }, e);
    }

}
