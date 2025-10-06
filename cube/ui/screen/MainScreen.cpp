#include "MainScreen.hpp"

namespace cube {

    MainScreen::MainScreen() = default;
    MainScreen::~MainScreen() = default;

    void MainScreen::onInit() {
//        font_handle = m_assets->load<Font>("monocraft.ttf", 32);
    }

    void MainScreen::onDeinit() {
//        m_assets->unload(font_handle); //if it's not needed - unload asset, else asset will be available until asset manager gets destructed
//        m_assets->unloadAll();       // if need to unload all assets
    }

    void MainScreen::onDraw(Context& ctx) {
        clear(Color(0xFF9a62cd));

/*        auto& ctx2d = ctx->get2D();

        ctx2d.begin();
        ctx2d.drawText(m_assets.get<Font>(font_handle), "Hello World!", {100, 100});
        ctx2d.end();*/
    }

    void MainScreen::onTick(float) {

    }

    void MainScreen::onEvent(const Event&) {

    }

}