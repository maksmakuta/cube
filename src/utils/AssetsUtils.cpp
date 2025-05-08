#include "cube/utils/AssetsUtils.hpp"

#ifndef CUBE_ASSETS_DIR
#error "CUBE_ASSETS_DIR is not defined, please define assets dir"
#endif

namespace cube {
    std::string getAsset(const std::string& path) {
        return CUBE_ASSETS_DIR + path;
    }
}