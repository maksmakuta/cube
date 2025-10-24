#include "game/Cube.hpp"
#include "window/Window.hpp"

int main(){
    const auto window = std::make_unique<cube::Window>("Cube");
    const auto game = std::make_unique<cube::Cube>();
    auto last = cube::getTime();
    while (!window->isClosed()) {
        cube::update();
        while(window->isNextEvent()) {
            game->onEvent(window->getEvent());
        }
        const auto now = cube::getTime();
        const auto delta = static_cast<float>(now - last);
        last = now;
        game->onUpdate(delta);
        game->onDraw();
        window->swapBuffers();
    }
}
