#ifndef CUBE_TEXTUREBUILDER_HPP
#define CUBE_TEXTUREBUILDER_HPP

#include <string>
#include <vector>

#include "Texture.hpp"
#include "TextureArray.hpp"
#include "TextureParams.hpp"

namespace cube {

    class TextureBuilder {
    public:
        TextureBuilder();
        ~TextureBuilder();

        TextureBuilder& setSize(const glm::ivec2&);
        TextureBuilder& setSize(const glm::ivec3&);
        TextureBuilder& setFormat(const TextureFormat&);
        TextureBuilder& setFilter(const TextureFilter&);
        TextureBuilder& setFilter(const TextureFilter&,const TextureFilter&);
        TextureBuilder& setWrap(const TextureWrap&);
        TextureBuilder& setWrap(const TextureWrap&,const TextureWrap&);
        TextureBuilder& generateMipmaps(bool);
        TextureBuilder& setFlipY(bool);

        TextureArray buildArray(const std::string& dir_path);
        TextureArray buildArray(const std::vector<std::string>& items);
        TextureArray buildArray();

        Texture build(const std::string& file_path);
        Texture build(const std::vector<uint8_t>& data, const glm::ivec2& size);
        Texture build();

    private:
        void applyParams(GLenum target) const;

        glm::ivec3 m_size{0,0,0};
        TextureFormat m_format = TextureFormat::RGBA8;
        TextureFilter m_minFilter = TextureFilter::Linear;
        TextureFilter m_magFilter = TextureFilter::Linear;
        TextureWrap m_wrapS = TextureWrap::ClampEdge;
        TextureWrap m_wrapT = TextureWrap::ClampEdge;
        bool m_flip = false;
        bool m_mipmap = false;
    };

}

#endif //CUBE_TEXTUREBUILDER_HPP