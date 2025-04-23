#include "cube/utils/AssetsUtils.hpp"

namespace cube {
    std::string getAsset(const std::string& path) {
        return CUBE_ASSETS_DIR + path;
    }
}