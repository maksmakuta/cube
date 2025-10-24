#ifndef CUBE_CHUNKRENDERER_HPP
#define CUBE_CHUNKRENDERER_HPP

#include "ChunkObject.hpp"
#include "graphics/Shader.hpp"
#include "graphics/TextureArray.hpp"
#include "models/world/World.hpp"

namespace cube {

    class ChunkRenderer {
    public:
        ChunkRenderer();
        ~ChunkRenderer();

        bool exists(const ChunkPos&) const;
        void add(const ChunkPos&, const ChunkObject&);
        void remove(const ChunkPos&);

        void resize(const glm::vec2&, float fov = 45.f);
        void draw(const glm::mat4&);

    private:
        Shader m_shader;
        TextureArray m_textures;
        glm::mat4 m_projection{1.f};
        std::unordered_map<ChunkPos, ChunkObject> m_chunks;
    };
}

#endif //CUBE_CHUNKRENDERER_HPP