#include "WorldRenderer.hpp"

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

    WorldRenderer::WorldRenderer(const int seed) : m_generator(seed){
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
    }

void WorldRenderer::update(const Camera& cam) {

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

        // for (const auto &[pos, renderable]: m_renderables) {
        //     if (renderable.count == 0) {
        //         m_dirty.insert(pos);
        //         continue;
        //     }
        //     m_shader.setMat4("model", renderable.model);
        //     glBindVertexArray(renderable.vao);
        //     glDrawElements(GL_TRIANGLES, renderable.count, GL_UNSIGNED_INT, nullptr);
        // }
        // glBindVertexArray(0);
    }

}
