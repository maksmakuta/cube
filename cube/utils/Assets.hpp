#ifndef CUBE_ASSETS_HPP
#define CUBE_ASSETS_HPP

#include <array>
#include <string>
#include <vector>
#include "graphics/FontStyle.h"

namespace cube {

    class Assets{
    public:
        Assets();

        static std::string getPath();
        static std::string getTexturesPath();
        static std::string getFontsPath();
        static std::string getShadersPath();

        static std::string getTexture(const std::string& name);
        static std::vector<std::string> getTextures(const std::string& dir);
        static std::string getFont(const FontStyle& style);
        static std::array<std::string,2> getShader(const std::string& name);
    };

}

#endif //CUBE_ASSETS_HPP