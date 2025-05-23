#include "cube/Window.hpp"

using namespace cube;

int main() {
    WindowParams params{};
    params.gl_major = 4;
    params.gl_minor = 6;
    params.gl_samples = 8;

    const auto cube = std::make_shared<Cube>();

    Window w(params);
    w.attach(cube);
    w.run();

    return 0;
}
