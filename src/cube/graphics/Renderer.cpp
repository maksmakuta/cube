#include "cube/graphics/Renderer.hpp"

#include <assets_dir.hpp>
#include <format>

#include "spng.h"
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

    Renderable toRenderable(const ChunkMesh& mesh) {
        //TODO(implement)
        return {};
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
        m_meshes[pos] = toRenderable(mesh);
    }

    void Renderer::draw(const glm::mat4& view, const glm::mat4& projection) {
        m_shader.use();

        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textures);

        m_shader.setMat4("u_proj", projection);
        m_shader.setMat4("u_view", view);

        for (const auto& [pos, renderable] : m_meshes) {
            if (renderable.count == 0) {
                continue;
            }
            m_shader.setMat4("u_model", glm::translate(glm::mat4(), glm::vec3(pos * CHUNK_SIZE)));
            glBindVertexArray(renderable.vao);
            glDrawArrays(GL_TRIANGLES, 0, renderable.count);
        }
    }

    void Renderer::loadTextures(const glm::ivec2& tileSize) {
        glGenTextures(1, &m_textures);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textures);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, tileSize.x, tileSize.y, LAYERS.size());

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
            info("Layer {} loaded with index {}", layer, index);
        }
    }

}
