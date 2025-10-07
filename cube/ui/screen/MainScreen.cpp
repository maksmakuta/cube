#include "MainScreen.hpp"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "utils/LambdaVisitor.hpp"

namespace cube {

    MainScreen::MainScreen() = default;
    MainScreen::~MainScreen() = default;

    void MainScreen::onInit() {

    }

    void MainScreen::onDeinit() {

    }

    void MainScreen::onDraw(Context& ctx) {
        clear(Color(0x00A0A0FF));

        const auto ctx2d = ctx.getRenderer2D();
        if (!ctx2d) {
            std::cout << "No 2D renderer found" << std::endl;
            return;
        }

        ctx2d->begin();
        ctx2d->fill(Color(0xFF0080FF));
        ctx2d->circle(m_view / 2.f, m_size);
        ctx2d->stroke(Color(0xFFFF8000));
        const auto len = m_size;
        const auto s = glm::vec2(len, len);
        ctx2d->rect(m_view / 2.f - s, s * 2.f);

        ctx2d->end();
    }

    void MainScreen::onTick(float) {

    }

    void MainScreen::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re) {
                m_view = glm::vec2(re.width, re.height);
            },
            [this](const KeyEvent& ke) {
                if (ke.pressed) {
                    if (ke.key == Key::W) {
                        m_size += 1.5f;
                        m_size = std::min(m_size, 450.0f);
                    }
                    if (ke.key == Key::S) {
                        m_size -= 1.5f;
                        m_size = std::max(m_size, 10.0f);
                    }
                }
            },
            [](const MouseEvent& me) {},
            [](const ScrollEvent& se) {},
            [](const InputEvent& ie) {}
        }, e);
    }

}
