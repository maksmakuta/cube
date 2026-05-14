#include "cube/graphics/Renderer.hpp"

#include <assets_dir.hpp>
#include <format>
#include <ranges>

#include "spng.h"
#include "cube/data/Chunk.hpp"
#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"
#include "glad/glad.h"

namespace cube {





    const auto LAYERS = std::vector<std::string>{
        "natural/grass_block_side.png",
        "natural/grass_block_side_overlay.png",
        "natural/grass_block_snow.png",
        "natural/grass_block_top.png",
        "natural/amethyst_block.png",
        "natural/andesite.png",
        "natural/basalt_side.png",
        "natural/basalt_top.png",
        "natural/bedrock.png",
        "natural/blackstone.png",
        "natural/blackstone_top.png",
        "natural/blue_ice.png",
        "natural/calcite.png",
        "natural/clay.png",
        "natural/coarse_dirt.png",
        "natural/cobblestone.png",
        "natural/deepslate.png",
        "natural/deepslate_top.png",
        "natural/diorite.png",
        "natural/dirt.png",
        "natural/dripstone_block.png",
        "natural/granite.png",
        "natural/gravel.png",
        "natural/ice.png",
        "natural/mud.png",
        "natural/packed_ice.png",
        "natural/red_sand.png",
        "natural/red_sandstone.png",
        "natural/sand.png",
        "natural/sandstone.png",
        "natural/stone.png",
        "natural/tuff.png",
        "lava_still.png",
        "water_still.png",
    };

    Renderable toRenderable(const ChunkMesh& mesh, const glm::ivec3& pos) {
        Renderable r = {};
        r.count = static_cast<int>(mesh.indices.size());
        r.min = glm::vec3(pos * CHUNK_SIZE);
        r.max = r.min + glm::vec3(CHUNK_SIZE);

        glCreateBuffers(1, &r.vbo);
        glCreateBuffers(1, &r.ebo);

        glNamedBufferStorage(r.vbo, static_cast<GLsizeiptr>(mesh.vertices.size() * sizeof(Vertex)), mesh.vertices.data(), 0);
        glNamedBufferStorage(r.ebo, static_cast<GLsizeiptr>(mesh.indices.size() * sizeof(uint32_t)), mesh.indices.data(), 0);

        glCreateVertexArrays(1, &r.vao);

        glVertexArrayVertexBuffer(r.vao, 0, r.vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(r.vao, r.ebo);

        glEnableVertexArrayAttrib(r.vao, 0);
        glVertexArrayAttribIFormat(r.vao, 0, 2, GL_UNSIGNED_INT, 0);
        glVertexArrayAttribBinding(r.vao, 0, 0);

        return r;
    }

    void clear(const Renderable& r) {
        glDeleteVertexArrays(1, &r.vao);
        glDeleteBuffers(1, &r.vbo);
        glDeleteBuffers(1, &r.ebo);
    }

    Renderer::Renderer() : m_shader("cube"), m_skyShader("celestial") {
        loadTextures();
        initSkyVAO();

        m_shader.use();
        m_shader.setFloat("u_minFog", RENDER_DIST * CHUNK_SIZE * 0.75f);
        m_shader.setFloat("u_maxFog", RENDER_DIST * CHUNK_SIZE * 0.9f);
    }

    Renderer::~Renderer() {
        glDeleteTextures(1, &m_textures);
    }

    bool Renderer::contains(const glm::ivec3& pos) const {
        return m_meshes.contains(pos);
    }

    void Renderer::remove(const glm::ivec3& pos) {
        if (contains(pos)) {
            m_meshes.erase(pos);
        }
    }

    void Renderer::push(const glm::ivec3& pos, const ChunkMesh& mesh) {
        if (!mesh.indices.empty()) {
            m_meshes[pos] = toRenderable(mesh, pos);
        }
    }

    struct Plane {
        glm::vec3 normal;
        float distance;

        void normalize() {
            const float length = glm::length(normal);
            normal /= length;
            distance /= length;
        }
    };

    struct Frustum {
        Plane planes[6];

        void update(const glm::mat4& vp) {
            planes[0] = { {vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0]}, vp[3][3] + vp[3][0] };
            planes[1] = { {vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0]}, vp[3][3] - vp[3][0] };
            planes[2] = { {vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1]}, vp[3][3] + vp[3][1] };
            planes[3] = { {vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1]}, vp[3][3] - vp[3][1] };
            planes[4] = { {vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2]}, vp[3][3] + vp[3][2] };
            planes[5] = { {vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2]}, vp[3][3] - vp[3][2] };

            for (auto & plane : planes) plane.normalize();
        }

        [[nodiscard]] bool isBoxVisible(const glm::vec3& min, const glm::vec3& max) const {
            for (const auto&[normal, distance] : planes) {

                glm::vec3 positiveCorner = min;
                if (normal.x >= 0) positiveCorner.x = max.x;
                if (normal.y >= 0) positiveCorner.y = max.y;
                if (normal.z >= 0) positiveCorner.z = max.z;

                if (glm::dot(normal, positiveCorner) + distance < 0) {
                    return false;
                }
            }
            return true;
        }

    };

    void Renderer::setSun(const glm::vec3& direction, const float angle) {
        m_shader.setVec3("u_sunDir", direction);
        m_shader.setFloat("u_sunAngle", angle);
        m_sunDir = direction;
    }

    void Renderer::setSkyColor(const glm::vec3& skyColor) {
        m_shader.setVec3("u_skyColor", skyColor);
    }

    void Renderer::draw(const glm::mat4& view, const glm::mat4& projection, const float time) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        drawSkyElements(projection, view, m_sunDir);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        m_shader.use();

        Frustum frustum{};
        frustum.update(projection * view);

        glBindTextureUnit(0, m_textures);

        m_shader.setInt("u_textures", 0);
        m_shader.setFloat("u_time", time);

        m_shader.setMat4("u_proj", projection);
        m_shader.setMat4("u_view", view);

        for (const auto& renderable : m_meshes | std::views::values) {
            if (renderable.count == 0) continue;
            if (!frustum.isBoxVisible(renderable.min, renderable.max)) continue;

            m_shader.setVec3("u_model", renderable.min);

            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, renderable.count, GL_UNSIGNED_INT, nullptr);
        }
    }

    void Renderer::loadTextures(const glm::ivec2& tileSize) {
        int totalLayers = 0;
        std::vector<std::vector<unsigned char>> pixelData;

        for (const auto& layerName : LAYERS) {
            std::string path = std::format("{}/textures/{}", ASSETS_PATH, layerName);
            FILE* fp = fopen(path.c_str(), "rb");
            if (!fp) continue;

            spng_ctx* ctx = spng_ctx_new(0);
            spng_set_png_file(ctx, fp);

            spng_ihdr ihdr{};
            spng_get_ihdr(ctx, &ihdr);

            size_t out_size;
            spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);
            std::vector<unsigned char> buffer(out_size);
            spng_decode_image(ctx, buffer.data(), out_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS);

            const int framesInThisFile = static_cast<int>(ihdr.height) / tileSize.y;
            totalLayers += framesInThisFile;

            pixelData.push_back(std::move(buffer));

            spng_ctx_free(ctx);
            fclose(fp);

            info("Texture {} have {} frames", path,  framesInThisFile);
        }

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_textures);

        const auto levels = static_cast<int>(std::floor(std::log2(std::max(tileSize.x, tileSize.y)))) + 1;

        glTextureStorage3D(m_textures, levels, GL_RGBA8, tileSize.x, tileSize.y, totalLayers);

        int currentLayerIndex = 0;
        for (auto & i : pixelData) {
            const int frames = static_cast<int>(i.size()) / (tileSize.x * 4) / tileSize.y;

            for (int f = 0; f < frames; ++f) {
                const void* framePtr = i.data() + (f * tileSize.x * tileSize.y * 4);

                glTextureSubImage3D(m_textures, 0, 0, 0, currentLayerIndex,
                                    tileSize.x, tileSize.y, 1,
                                    GL_RGBA, GL_UNSIGNED_BYTE, framePtr);

                currentLayerIndex++;
            }
        }

        glTextureParameteri(m_textures, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTextureParameteri(m_textures, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        float maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        glTextureParameterf(m_textures, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);

        glGenerateTextureMipmap(m_textures);
    }

    int Renderer::clearChunks(const glm::ivec3& pos, const int dist) {
        int clearedCount = 0;

        for (auto it = m_meshes.begin(); it != m_meshes.end(); ) {
            const glm::ivec3& chunkPos = it->first;

            const bool outOfRange = std::abs(chunkPos.x - pos.x) > dist ||
                              std::abs(chunkPos.y - pos.y) > dist ||
                              std::abs(chunkPos.z - pos.z) > dist;

            if (outOfRange) {
                auto& mesh = it->second;
                clear(mesh);

                it = m_meshes.erase(it);
                clearedCount++;
            } else {
                ++it;
            }
        }
        return clearedCount;
    }

    struct SkyVertex {
        float x, y, z;
        float u, v;
    };

    SkyVertex skyVertices[] = {
        {-1.0f, -1.0f, 0.0f,  0.0f, 0.0f},
        { 1.0f, -1.0f, 0.0f,  1.0f, 0.0f},
        { 1.0f,  1.0f, 0.0f,  1.0f, 1.0f},

        {-1.0f, -1.0f, 0.0f,  0.0f, 0.0f},
        { 1.0f,  1.0f, 0.0f,  1.0f, 1.0f},
        {-1.0f,  1.0f, 0.0f,  0.0f, 1.0f}
    };

    void Renderer::initSkyVAO() {
        glCreateVertexArrays(1, &m_skyVAO);
        glCreateBuffers(1, &m_skyVBO);

        glNamedBufferStorage(m_skyVBO, sizeof(skyVertices), skyVertices, 0);

        glEnableVertexArrayAttrib(m_skyVAO, 0);
        glVertexArrayAttribFormat(m_skyVAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(SkyVertex, x));
        glVertexArrayAttribBinding(m_skyVAO, 0, 0);

        glEnableVertexArrayAttrib(m_skyVAO, 1);
        glVertexArrayAttribFormat(m_skyVAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(SkyVertex, u));
        glVertexArrayAttribBinding(m_skyVAO, 1, 0);

        glVertexArrayVertexBuffer(m_skyVAO, 0, m_skyVBO, 0, sizeof(SkyVertex));
    }

    void Renderer::drawSkyElements(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& sunDir) {
        m_skyShader.use();

        const auto staticView = glm::mat4(glm::mat3(view));
        m_skyShader.setMat4("u_view", staticView);
        m_skyShader.setMat4("u_proj", projection);

        glBindVertexArray(m_skyVAO);

        constexpr float sunDist = 50.0f;
        constexpr float sunSize = 4.0f;
        m_skyShader.setVec3("u_pos", sunDir * sunDist);
        m_skyShader.setFloat("u_size", sunSize);
        m_skyShader.setVec3("u_color", glm::vec3(1.0f, 1.0f, 0.8f));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        constexpr float moonDist = 50.0f;
        constexpr float moonSize = 3.0f;
        m_skyShader.setVec3("u_pos", -sunDir * moonDist);
        m_skyShader.setFloat("u_size", moonSize);
        m_skyShader.setVec3("u_color", glm::vec3(0.9f, 0.9f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

}
