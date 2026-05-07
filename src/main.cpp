#include <cstring>

#include "cube/utils/Log.hpp"

int main(const int argc, const char** argv) {
    cube::info("Starting {}", argv[0]);
    cube::warn("Length {}", std::strlen(argv[0]));
    cube::debug("Hello, World!");
    cube::error("Error");
    return 0;
}
