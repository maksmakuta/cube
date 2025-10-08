#include "MainScreen.hpp"

#include "graphics/TextureBuilder.hpp"
#include "utils/AssetsPaths.hpp"
#include "utils/LambdaVisitor.hpp"

namespace cube {

    MainScreen::MainScreen() = default;
    MainScreen::~MainScreen() = default;

    void MainScreen::onInit() {
        loadTexture();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void MainScreen::onDeinit() {}

    void MainScreen::onDraw(Context& ctx) {
        clear(Color(0xFF202020));
        if (const auto ctx2d = ctx.getRenderer2D()) {
            onDraw2D(*ctx2d);
        }
    }

    void MainScreen::onDraw2D(Renderer2D &r) const {
        constexpr auto size = glm::vec2{256,256};

        r.begin();
        r.fill(dirt);
        r.rect(m_view / 2.f - size / 2.f, size);
        r.end();
    }

    void MainScreen::loadTexture() {
        if (id < 0 || id >= 10) return;
        dirt = TextureBuilder()
            .setFilter(TextureFilter::Nearest)
            .build(getTexture("blocks/" + std::to_string(id) + ".png"));
    }

    void MainScreen::onTick(float) {

    }

    void MainScreen::onEvent(const Event& e) {
        std::visit(LambdaVisitor{
            [this](const ResizeEvent& re) {
                m_view = {re.width, re.height};
            },
            [this](const KeyEvent& ke) {
                if (ke.pressed) {
                    if (ke.key == Key::W) {
                        id++;
                        loadTexture();
                    }
                    if (ke.key == Key::S) {
                        id--;
                        loadTexture();
                    }
                }
            },
            [](const MouseEvent&) {},
            [](const ScrollEvent&) {},
            [](const InputEvent&) {}
        }, e);
    }

}
