#include "cube/Cube.hpp"

int main(){
    const auto cube = std::make_shared<cube::Cube>();
    while (cube->isRunning()) {
        cube->update();
        cube->render();
    }
    return 0;
}
