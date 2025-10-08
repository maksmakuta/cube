#ifndef CUBE_TEXTUREBUILDER_HPP
#define CUBE_TEXTUREBUILDER_HPP

#include <string>

#include "Texture.hpp"
#include "TextureArray.hpp"
#include "TextureParams.hpp"

namespace cube {

    class TextureBuilder {
    public:
        TextureBuilder();
        ~TextureBuilder() = default;

        TextureBuilder& setSize(const glm::ivec2&);
        TextureBuilder& setFormat(const TextureFormat&);
        TextureBuilder& setFilter(const TextureFilter&);
        TextureBuilder& setFilter(const TextureFilter&,const TextureFilter&);
        TextureBuilder& setWrap(const TextureWrap&);
        TextureBuilder& setWrap(const TextureWrap&,const TextureWrap&);
        TextureBuilder& setFlipY(bool);

        TextureArray buildArray(const std::string& dir_path);
        TextureArray buildArray();

        Texture build(const std::string& file_path);
        Texture build();
    private:
        void applyParams(GLenum target) const;

        int m_width = 1;
        int m_height = 1;
        TextureFormat m_format = TextureFormat::RGBA8;
        TextureFilter m_minFilter = TextureFilter::Linear;
        TextureFilter m_magFilter = TextureFilter::Linear;
        TextureWrap m_wrapS = TextureWrap::ClampEdge;
        TextureWrap m_wrapT = TextureWrap::ClampEdge;
        bool m_flip = false;
    };

}

#endif //CUBE_TEXTUREBUILDER_HPP