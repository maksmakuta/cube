#include "WorldRenderer.hpp"

#include <ranges>
#include <unordered_set>

#include "Color.hpp"
#include "TextureBuilder.hpp"
#include "utils/AssetsPaths.hpp"

namespace cube {

    constexpr auto VIEW_DISTANCE = 12;

    constexpr ChunkPos DIRECTIONS[4] = {
        {  1,  0 },
        { -1,  0 },
        {  0,  1 },
        {  0, -1 },
    };

    WorldRenderer::WorldRenderer(const uint32_t seed) : m_generator(seed){
        m_shader.fromName("render3d");
        m_textures = TextureBuilder()
                .generateMipmaps(true)
                .setFilter(TextureFilter::NearestNearest, TextureFilter::Nearest)
                .buildArray(getTexture("/blocks/"));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    WorldRenderer::~WorldRenderer(){
        m_shader.release();
        m_textures.release();
        for (auto &renderable: m_renderables | std::views::values) {
            renderable.release();
        }
        m_renderables.clear();
    }

void WorldRenderer::update(const Camera& cam) {
        const glm::ivec2 camChunk{
            static_cast<int>(std::floor(cam.getPosition().x / CHUNK_SIZE.x)),
            static_cast<int>(std::floor(cam.getPosition().z / CHUNK_SIZE.z))
        };

        std::unordered_set<ChunkPos> wantedChunks;
        wantedChunks.reserve((2 * VIEW_DISTANCE + 1) * (2 * VIEW_DISTANCE + 1));

        for (int dx = -VIEW_DISTANCE; dx <= VIEW_DISTANCE; ++dx)
            for (int dz = -VIEW_DISTANCE; dz <= VIEW_DISTANCE; ++dz)
                wantedChunks.insert({ camChunk.x + dx, camChunk.y + dz });


        for (const auto& pos : wantedChunks) {
            if (!m_renderables.contains(pos)) {
                if (ChunkPtr chunk = m_world.getChunk(pos); !chunk) {
                    chunk = m_generator.generateChunk(pos);
                    m_world.setChunk(pos, chunk);
                }
            }
        }

        for (auto it = m_renderables.begin(); it != m_renderables.end(); ) {
            if (!wantedChunks.contains(it->first)) {
                m_free_list.emplace_back(it->second);
                it = m_renderables.erase(it);
            } else {
                ++it;
            }
        }

        for (const auto& pos : wantedChunks) {
            if (m_renderables.contains(pos)) {
                continue;
            }
            ChunkPtr chunk = m_world.getChunk(pos);

            std::vector<ChunkPtr> neighbours;
            neighbours.reserve(4);
            for (const auto& dir : DIRECTIONS)
                neighbours.push_back(m_world.getChunk(pos + dir));

            // Allocate renderable
            Renderable r{};
            if (m_free_list.empty()) {
                r = toRenderable(pos, chunk, neighbours);
            } else {
                r = m_free_list.back();
                m_free_list.pop_back();
                r.update(toMesh(chunk, neighbours));
                r.model = glm::translate(glm::mat4{1.f}, glm::vec3{CHUNK_SIZE.x * pos.x, 0.f, CHUNK_SIZE.z * pos.y});
            }

            m_renderables[pos] = r;
        }
    }

    void WorldRenderer::resize(const glm::vec2& size, const float fov){
        const auto wi = static_cast<int>(size.x);
        const auto hi = static_cast<int>(size.y);
        m_projection = glm::perspective(glm::radians(fov), size.x / size.y, 0.1f, static_cast<float>(CHUNK_SIZE.y * VIEW_DISTANCE));
        glViewport(0, 0, wi, hi);
    }

    void WorldRenderer::render(const glm::mat4& view) {
        clear(0xFF404040);
        m_shader.use();
        m_shader.setMat4("proj", m_projection);
        m_shader.setMat4("view", view);

        m_textures.bind();
        m_shader.setInt("textures", 0);

        for (const auto& [pos,renderable]: m_renderables) {
            m_shader.setMat4("model", renderable.model);
            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, renderable.count, GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);
    }

}
