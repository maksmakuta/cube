#ifndef CUBE_ASSETSPATHS_HPP
#define CUBE_ASSETSPATHS_HPP

#include <string>

#ifndef ASSETS_PATH
#error "define ASSETS_PATH to know where assets files is located"
#endif

namespace cube {

    inline std::string getAssets() {
        return ASSETS_PATH;
    }

    inline std::string getTexture(const std::string& filename) {
        return std::string(ASSETS_PATH) + "/textures/" + filename;
    }

    inline std::string getShader(const std::string& filename) {
        return std::string(ASSETS_PATH) + "/shaders/" + filename;
    }

    inline std::string getFont(const std::string& filename) {
        return std::string(ASSETS_PATH) + "/fonts/" + filename;
    }

}

#endif //CUBE_ASSETSPATHS_HPP