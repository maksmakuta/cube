#include <memory>

#include "graphics/Color.hpp"
#include "window/Window.hpp"

int main(){
    const auto window = std::make_shared<cube::Window>("Cube");
    assert(window != nullptr);
    while (!window->isClosed()) {
        cube::clear(0xFF202020);
        window->swapBuffers();
        cube::update();
    }
    return 0;
}
