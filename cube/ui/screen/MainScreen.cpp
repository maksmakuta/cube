#include "MainScreen.hpp"

namespace cube {

    MainScreen::MainScreen() = default;
    MainScreen::~MainScreen() = default;

    void MainScreen::onInit() {

    }

    void MainScreen::onDeinit() {

    }

    void MainScreen::onDraw(Context& ctx) {
        clear(Color(0xFF9a62cd));
    }

    void MainScreen::onTick(float) {

    }

    void MainScreen::onEvent(const Event&) {

    }

}