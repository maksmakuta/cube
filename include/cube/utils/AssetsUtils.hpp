#ifndef ASSETSUTILS_HPP
#define ASSETSUTILS_HPP

#include <string>

#ifndef CUBE_ASSETS_DIR
#error "CUBE_ASSETS_DIR is not defined, please define assets dir"
#endif


namespace cube {

    std::string getAsset(const std::string& path);

}

#endif //ASSETSUTILS_HPP
