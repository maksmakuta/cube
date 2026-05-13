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
        "natural/grass_block_top.png",
        "natural/grass_block_side.png",
        "natural/grass_block_side_overlay.png",
        "natural/dirt.png",
        "natural/stone.png",
        "natural/bedrock.png",
    };

    Renderable toRenderable(const ChunkMesh& mesh, const glm::ivec3& pos) {
        Renderable r = {};
        r.count = static_cast<int>(mesh.indices.size());
        r.min = glm::vec3(pos * CHUNK_SIZE);
        r.max = r.min + glm::vec3(CHUNK_SIZE);

        glCreateBuffers(1, &r.vbo);
        glCreateBuffers(1, &r.ebo);

        glNamedBufferStorage(r.vbo, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), 0);
        glNamedBufferStorage(r.ebo, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), 0);

        glCreateVertexArrays(1, &r.vao);

        glVertexArrayVertexBuffer(r.vao, 0, r.vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(r.vao, r.ebo);

        glEnableVertexArrayAttrib(r.vao, 0);
        glVertexArrayAttribIFormat(r.vao, 0, 2, GL_UNSIGNED_INT, offsetof(Vertex, data));
        glVertexArrayAttribBinding(r.vao, 0, 0);

        return r;
    }

    void clear(const Renderable& r) {
        glDeleteVertexArrays(1, &r.vao);
        glDeleteBuffers(1, &r.vbo);
        glDeleteBuffers(1, &r.ebo);
    }

    Renderer::Renderer() : m_textures(0), m_shader("cube") {
        loadTextures();
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

    const float DAY_NIGHT_CYCLE_SECONDS = 60.0f;

    void Renderer::draw(const glm::mat4& view, const glm::mat4& projection, const float time) {
        const glm::vec3 skyNoon   = glm::vec3(0.45f, 0.7f, 1.0f);
        const glm::vec3 skySunset = glm::vec3(1.0f, 0.4f, 0.2f);
        const glm::vec3 skyNight  = glm::vec3(0.02f, 0.02f, 0.05f);

        float cycleProgress = std::fmod(time, DAY_NIGHT_CYCLE_SECONDS) / DAY_NIGHT_CYCLE_SECONDS;
        float sunAngle = cycleProgress * 2.0f * 3.14159265f;
        glm::vec3 sunDir = glm::normalize(glm::vec3(0.0f, glm::sin(sunAngle), glm::cos(sunAngle)));

        float sunHeight = sunDir.y;
        glm::vec3 currentSkyColor;

        if (sunHeight > 0.1f) {
            float t = glm::clamp((sunHeight - 0.1f) / 0.9f, 0.0f, 1.0f);
            currentSkyColor = glm::mix(skySunset, skyNoon, t);
        } else if (sunHeight > -0.1f) {
            float t = glm::clamp((sunHeight + 0.1f) / 0.2f, 0.0f, 1.0f);
            currentSkyColor = glm::mix(skyNight, skySunset, t);
        } else {
            currentSkyColor = skyNight;
        }

        glClearColor(currentSkyColor.r, currentSkyColor.g, currentSkyColor.b, 1.0f);

        m_shader.use();

        Frustum frustum{};
        frustum.update(projection * view);

        glBindTextureUnit(0, m_textures);
        m_shader.setInt("u_textures", 0);
        m_shader.setFloat("u_time", time);

        m_shader.setMat4("u_proj", projection);
        m_shader.setMat4("u_view", view);

        m_shader.setVec3("u_sunDir", sunDir);
        m_shader.setFloat("u_sunAngle", sunAngle);

        for (const auto& renderable : m_meshes | std::views::values) {
            if (renderable.count == 0) continue;
            if (!frustum.isBoxVisible(renderable.min, renderable.max)) continue;

            m_shader.setVec3("u_model", renderable.min);

            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, renderable.count, GL_UNSIGNED_INT, nullptr);
        }
    }

    void Renderer::loadTextures(const glm::ivec2& tileSize) {
        glGenTextures(1, &m_textures);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textures);

        glTexStorage3D(
            GL_TEXTURE_2D_ARRAY,
            4,
            GL_RGBA8,
            tileSize.x,
            tileSize.y,
            static_cast<GLsizei>(LAYERS.size()));

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        for (auto index = 0; index < LAYERS.size(); ++index) {
            const auto& layer = LAYERS[index];
            FILE *fp = fopen(std::format("{}/textures/{}", ASSETS_PATH, layer).c_str(), "rb");
            if(!fp) {
                warn("Cannot load layer {} at index {}", layer, index);
                return;
            }

            spng_ctx *ctx = spng_ctx_new(0);
            spng_set_png_file(ctx, fp);

            size_t out_size;
            spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);
            auto *buffer = static_cast<unsigned char *>(malloc(out_size));

            spng_decode_image(ctx, buffer, out_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS);

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, tileSize.x, tileSize.y, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

            free(buffer);
            spng_ctx_free(ctx);
            fclose(fp);
            debug("Layer {} loaded with index {}", layer, index);
        }

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

}
