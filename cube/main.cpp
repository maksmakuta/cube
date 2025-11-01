#include <memory>

#include "game/Cube.h"
#include "window/Window.hpp"

int main(){
    const auto window = std::make_shared<cube::Window>("Cube");
    const auto cube = std::make_unique<cube::Cube>();
    assert(window != nullptr);
    cube->onInit();
    auto time = cube::getTime();
    while (!window->isClosed()) {
        cube::update();
        while(window->isNextEvent()){
            cube->onEvent(window->getEvent());
        }

        auto now = cube::getTime();
        cube->onTick(static_cast<float>(now - time));
        time = now;

        cube->onDraw();
        window->swapBuffers();
    }
    cube->onDeinit();
    return 0;
}
