#include "Cube.hpp"

#include <format>
#include <iostream>
#include <random>

#include "graphics/Color.hpp"
#include "utils/AssetsPaths.hpp"
#include "utils/LambdaVisitor.hpp"

namespace cube {

    int randomSeed() {
        return static_cast<int>(std::mt19937{std::random_device{}()}());
    }

    Cube::Cube() :
        m_gen(randomSeed()),
        m_renderer(std::make_unique<ChunkRenderer>()),
        m_2drenderer(std::make_unique<Renderer2D>())
    {
        m_font.load(getFont("Monocraft-Regular.ttf"),24);
    }

    Cube::~Cube() = default;

    void Cube::onDraw() {
        clear(0xFF353535);
        m_renderer->draw(m_camera.getMatrix());
            onDrawUI(*m_2drenderer);
    }

    void Cube::onDrawUI(const Renderer2D& ctx) {
        ctx.begin();
        ctx.text(m_font, Color(0xFFFFFFFF));
        ctx.print(std::format("Seed: {}", m_gen.getSeed()), {0, m_font.getSize()});
        ctx.end();
    }

    void Cube::onUpdate(const float dt) {
        m_camera.update(dt);
    }

    void Cube::onEvent(const Event& e) {
        m_camera.onEvent(e);
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re) {
                m_renderer->resize({re.width, re.height},m_camera.getFOV());
                m_2drenderer->resize({re.width, re.height});
            },
            [](const MouseEvent&) {},
            [this](const KeyEvent& ke) {
                if (ke.pressed && ke.key == Key::F1) {
                    is_debug = !is_debug;
                }
            },
            [](const ScrollEvent&) {},
            [](const InputEvent&) {},
        }, e);
    }

}
