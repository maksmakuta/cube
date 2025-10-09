#include "MainScreen.hpp"

#include <iostream>
#include <ostream>

#include "graphics/TextureBuilder.hpp"
#include "utils/AssetsPaths.hpp"
#include "utils/LambdaVisitor.hpp"

namespace cube {

    MainScreen::MainScreen() = default;
    MainScreen::~MainScreen() = default;

    void MainScreen::onInit() {
        std::cout << "MainScreen::onInit" << std::endl;
        font.load(getFont("Monocraft-Regular.ttf"), 32);
    }

    void MainScreen::onDeinit() {
        std::cout << "MainScreen::onDeinit" << std::endl;
    }

    void MainScreen::onDraw(Context& ctx) {
        clear(Color(0xFFA0A0A0));
        if (const auto ctx2d = ctx.getRenderer2D()) {
            onDraw2D(*ctx2d);
        }
    }

    void MainScreen::onDraw2D(Renderer2D &r) {
        r.begin();
        // r.stroke(Color(0xFFFF2025), 10);
        // r.rect({100,100},{512,512});
        // r.flush();
        r.fill(font.getTexture());
        r.rect({100,100},{512,512});
        r.end();
    }

    void MainScreen::onTick(float) {

    }

    void MainScreen::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re) {
                m_view = {re.width, re.height};
            },
            [](const KeyEvent&) {},
            [](const MouseEvent&) {},
            [](const ScrollEvent&) {},
            [](const InputEvent&) {}
        }, e);
    }

}
