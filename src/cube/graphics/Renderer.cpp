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
        Renderable renderable = {};
        renderable.count = static_cast<int>(mesh.indices.size());
        renderable.min = glm::vec3(pos * CHUNK_SIZE);
        renderable.max = renderable.min + glm::vec3(CHUNK_SIZE);
        glGenVertexArrays(1, &renderable.vao);
        glGenBuffers(1, &renderable.vbo);
        glGenBuffers(1, &renderable.ebo);
        glBindVertexArray(renderable.vao);
        glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, data)));
        glBindVertexArray(0);

        return renderable;
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

    void Renderer::draw(const glm::mat4& view, const glm::mat4& projection) {
        m_shader.use();

        Frustum frustum{};
        frustum.update(projection * view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textures);
        m_shader.setInt("textures", 0);

        m_shader.setMat4("u_proj", projection);
        m_shader.setMat4("u_view", view);

        for (const auto &renderable: m_meshes | std::views::values) {
            if (renderable.count == 0)
                continue;

            if (!frustum.isBoxVisible(renderable.min, renderable.max))
                continue;

            m_shader.setVec3("u_model", renderable.min);
            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, renderable.count, GL_UNSIGNED_INT, nullptr);
        }

        glBindVertexArray(0);
    }

    void Renderer::loadTextures(const glm::ivec2& tileSize) {
        glGenTextures(1, &m_textures);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textures);

        glTexStorage3D(
            GL_TEXTURE_2D_ARRAY,
            1,
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
