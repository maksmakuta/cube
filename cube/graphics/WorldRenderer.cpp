#include "WorldRenderer.hpp"

#include <ranges>

#include "Color.hpp"
#include "TextureBuilder.hpp"
#include "utils/AssetsPaths.hpp"

namespace cube {

    constexpr auto VIEW_DISTANCE = 8;

    WorldRenderer::WorldRenderer(const uint32_t seed) : m_generator(seed){
        m_shader.fromName("render3d");
        m_textures = TextureBuilder()
                .setFilter(TextureFilter::Nearest)
                .buildArray(getTexture("/blocks/"));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        constexpr auto pos = glm::ivec2{0,0};
        const ChunkPtr chunk = m_generator.generateChunk(pos);
        m_world.setChunk(pos, chunk);
        m_renderables[pos] = toRenderable(chunk,pos);
    }

    WorldRenderer::~WorldRenderer(){
        m_shader.release();
        m_textures.release();
        for (auto &renderable: m_renderables | std::views::values) {
            renderable.release();
        }
        m_renderables.clear();
    }

    void WorldRenderer::update(const glm::vec3& center){/*
        const glm::ivec2 camChunk{
            static_cast<int>(std::floor(center.x / CHUNK_SIZE.x)),
            static_cast<int>(std::floor(center.z / CHUNK_SIZE.z))
        };

        std::unordered_set<ChunkPos> wantedChunks;

        for(int dx=-VIEW_DISTANCE; dx<=VIEW_DISTANCE; ++dx)
            for(int dz=-VIEW_DISTANCE; dz<=VIEW_DISTANCE; ++dz){
                ChunkPos pos = { camChunk.x + dx, camChunk.y + dz };
                wantedChunks.insert(pos);

                if(!m_renderables.contains(pos)){
                    ChunkPtr chunk = m_world.getChunk(pos);
                    if(!chunk){
                        chunk = m_generator.generateChunk(pos);
                        m_world.setChunk(pos, chunk);
                    }

                    const Renderable r = toRenderable(chunk,pos);
                    m_renderables[pos] = r;
                }
            }

        for(auto it = m_renderables.begin(); it != m_renderables.end(); ){
            if(!wantedChunks.contains(it->first)){
                it->second.release();
                it = m_renderables.erase(it);
            }
            else ++it;
        }
    */}

    void WorldRenderer::resize(const glm::vec2& size, const float fov){
        const auto wi = static_cast<int>(size.x);
        const auto hi = static_cast<int>(size.y);
        m_projection = glm::perspective(glm::radians(fov), size.x / size.y, 0.1f, static_cast<float>(CHUNK_SIZE.y * VIEW_DISTANCE));
        glViewport(0, 0, wi, hi);
    }

    void WorldRenderer::render(const glm::mat4& view){
        clear(0xFF404040);
        m_shader.use();
        m_shader.setMat4("proj", m_projection);
        m_shader.setMat4("view", view);

        m_textures.bind();
        m_shader.setInt("textures", 0);

        for (const auto &renderable : m_renderables | std::views::values) {
            m_shader.setMat4("model", renderable.model);
            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, renderable.count, GL_UNSIGNED_INT, nullptr);

        }
        glBindVertexArray(0);
    }

}
