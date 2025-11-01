#include "Assets.hpp"

#include <filesystem>

#ifndef ASSETS_PATH
#error "define ASSETS_PATH to know where assets files is located"
#endif

namespace cube{

    Assets::Assets() = default;

    std::string Assets::getPath(){
        return ASSETS_PATH;
    }

    std::string Assets::getTexturesPath(){
        return getPath() + "/textures/";
    }

    std::string Assets::getFontsPath(){
        return getPath() + "/fonts/";
    }

    std::string Assets::getShadersPath(){
        return getPath() + "/shaders/";
    }

    std::string Assets::getTexture(const std::string& name){
        return getTexturesPath() + name;
    }

    std::vector<std::string> Assets::getTextures(const std::string& dir) {
        namespace fs = std::filesystem;

        auto list = std::vector<std::string>();
        const auto path = getTexture(dir);
        for (const auto &entry: fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                list.push_back(entry.path().string());
            }
        }
        return list;
    }

    std::string to_string(const FontStyle& f){
        switch (f) {
            case Regular:
                return "Regular";
            case BoldItalic:
                return "Bold-Italic";
            case Bold:
                return "Bold";
            case Italic:
                return "Italic";
            default:
                return "";
        }
    }

    std::string Assets::getFont(const FontStyle& style){
        return getFontsPath() + "Monocraft-" + to_string(style) + ".ttf";
    }

    std::array<std::string,2> Assets::getShader(const std::string& name){
        return {
            getShadersPath() + name + "_vert.glsl",
            getShadersPath() + name + "_frag.glsl"
        };
    }

}
