#include <cube/graphics/Renderer.hpp>

#include <expected>
#include <fstream>
#include <ranges>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <spng.h>
#include <SDL3/SDL_timer.h>

#include <cube/utils/Logger.hpp>
#include <cube/data/Chunk.hpp>

namespace cube {
    Renderer::Renderer() {
        loadShader();
        loadTextures();
    }

    Renderer::~Renderer() {
        if (m_shader) {
            glDeleteProgram(m_shader);
        }
        if (m_texture) {
            glDeleteTextures(1,&m_texture);
        }
    }

    std::expected<std::string, std::string> readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return std::unexpected("Failed to open file: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static std::expected<GLuint, std::string> compileShader(const GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            return std::unexpected(std::string("Compile Error: ") + infoLog);
        }
        return shader;
    }

    void Renderer::loadShader() {
        auto vertSrc = readFile("../assets/shaders/cube.vert");
        auto fragSrc = readFile("../assets/shaders/cube.frag");

        if (!vertSrc) {
            Log::error("Read vert shader file error: {}",vertSrc.error());
            return;
        }
        if (!fragSrc) {
            Log::error("Read frag shader file error: {}",fragSrc.error());
            return;
        }

        auto vShader = compileShader(GL_VERTEX_SHADER, vertSrc->c_str());
        if (!vShader) {
            Log::error("Compiling error <Vertex>: {}", vShader.error());
            return;
        }

        auto fShader = compileShader(GL_FRAGMENT_SHADER, fragSrc->c_str());
        if (!fShader) {
            Log::error("Compiling error <Fragment>: {}", fShader.error());
            glDeleteShader(*vShader);
            return;
        }

        m_shader = glCreateProgram();
        glAttachShader(m_shader, *vShader);
        glAttachShader(m_shader, *fShader);
        glLinkProgram(m_shader);

        GLint success;
        glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_shader, 512, nullptr, infoLog);
            Log::error("Linking shader program Error: {}",infoLog);
        }

        m_projLoc = glGetUniformLocation(m_shader, "projection");
        m_viewLoc = glGetUniformLocation(m_shader, "view");
        m_modelLoc = glGetUniformLocation(m_shader, "model");

        glDeleteShader(*vShader);
        glDeleteShader(*fShader);
    }

    void Renderer::loadTextures() {
        Log::info("Load textures");

        const std::vector<std::string> textureFiles = {
            "birch_leaves1.png",
            "cactus_bottom.png",
            "copper_ore.png",
            "grass1.png",
            "ice.png",
            "melon_bottom.png",
            "poppy.png",
            "sand.png",
            "sugar_cane.png",
            "birch_log.png",
            "cactus_flower.png",
            "dandelion.png",
            "grass_block_side_overlay.png",
            "iron_ore.png",
            "melon_side.png",
            "pumpkin_bottom.png",
            "snow.png",
            "tall_grass_bottom.png",
            "birch_log_top.png",
            "cactus_side.png",
            "diorite.png",
            "grass_block_side.png",
            "jungle_leaves.png",
            "melon_top.png",
            "pumpkin_side.png",
            "spruce_leaves1.png",
            "tall_grass_top.png",
            "brown_mushroom1.png",
            "cactus_top.png",
            "dirt.png",
            "grass_block_snow.png",
            "jungle_log.png",
            "oak_leaves.png",
            "pumpkin_top.png",
            "spruce_log.png",
            "tuff.png",
            "bubble_coral.png",
            "clay.png",
            "fern.png",
            "grass_block_top.png",
            "jungle_log_top.png",
            "oak_log.png",
            "red_mushroom.png",
            "spruce_log_top.png",
            "vine.png",
            "bush.png",
            "coal_ore.png",
            "gold_ore.png",
            "gravel.png",
            "kelp_plant.png",
            "oak_log_top.png",
            "red_tulip.png",
            "stone.png",
            "water_still.png",
        };

        constexpr int TEX_WIDTH = 16;

        struct DecodedImage {
            std::string name;
            std::vector<unsigned char> pixels;
            int frames;
        };

        std::vector<DecodedImage> decodedImages;
        int totalLayers = 0;

        for (const auto& file_name : textureFiles) {
            const std::string path = std::format("../assets/textures/{}", file_name);
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file) {
                Log::error("Failed to open texture: {}", path);
                continue;
            }

            const auto fileSize = file.tellg();
            std::vector<char> fileBuffer(fileSize);
            file.seekg(0);
            file.read(fileBuffer.data(), fileSize);

            spng_ctx* ctx = spng_ctx_new(0);
            spng_set_png_buffer(ctx, fileBuffer.data(), fileSize);

            spng_ihdr ihdr{};
            if (spng_get_ihdr(ctx, &ihdr) != 0) {
                Log::error("Failed to read PNG header: {}", path);
                spng_ctx_free(ctx);
                continue;
            }

            int frames = static_cast<int>(ihdr.height) / TEX_WIDTH;
            totalLayers += frames;

            size_t out_size;
            spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);
            std::vector<unsigned char> rgbaBuffer(out_size);

            if (const int ret = spng_decode_image(ctx, rgbaBuffer.data(), out_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS)) {
                Log::error("spng error: {}", spng_strerror(ret));
                spng_ctx_free(ctx);
                continue;
            }

            decodedImages.push_back({file_name, std::move(rgbaBuffer), frames});
            spng_ctx_free(ctx);
        }

        Log::info("Total texture layers calculated: {}", totalLayers);

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);

        glTextureStorage3D(m_texture, 1, GL_RGBA8, TEX_WIDTH, TEX_WIDTH, totalLayers);

        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        int currentLayer = 0;

        for (const auto& img : decodedImages) {
            glTextureSubImage3D(
                m_texture,
                0,
                0, 0, currentLayer,
                TEX_WIDTH, TEX_WIDTH, img.frames,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                img.pixels.data()
            );

            Log::info("Loaded: {} -> Mapped to layers [{} to {}]",
                      img.name, currentLayer, currentLayer + img.frames - 1);

            currentLayer += img.frames;
        }

        glGenerateMipmap(m_texture);
    }

    void Renderer::render(const glm::mat4& projection, const glm::mat4& view) {
        {
            std::lock_guard lock(m_trashMutex);
            for (const auto& pos : m_removed) {
                auto& r = m_renderables[pos];
                glDeleteVertexArrays(1, &r.solidVao);
                glDeleteBuffers(1, &r.solidVbo);
                glDeleteBuffers(1, &r.solidEbo);
                glDeleteVertexArrays(1, &r.transVao);
                glDeleteBuffers(1, &r.transVbo);
                glDeleteBuffers(1, &r.transEbo);
                m_renderables.erase(pos);
            }
            m_removed.clear();
        }

        glUseProgram(m_shader);

        const float timeSeconds = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        glUniform1f(glGetUniformLocation(m_shader, "uTime"), timeSeconds);
        glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, &view[0][0]);

        glBindTextureUnit(0, m_texture);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);

        for (const auto &r: m_renderables | std::views::values) {
            if (r.solidCount > 0) {
                glUniform3iv(m_modelLoc, 1, &r.model[0]);
                glBindVertexArray(r.solidVao);
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(r.solidCount), GL_UNSIGNED_INT, nullptr);
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDepthMask(GL_FALSE);

        for (const auto &r: m_renderables | std::views::values) {
            if (r.transCount > 0) {
                glUniform3iv(m_modelLoc, 1, &r.model[0]);
                glBindVertexArray(r.transVao);
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(r.transCount), GL_UNSIGNED_INT, nullptr);
            }
        }

        glDepthMask(GL_TRUE);
    }

    void Renderer::put(const RenderableMesh& mesh) {
        if (mesh.solidVertices.empty() && mesh.transVertices.empty()) return;

        Renderable r{};
        r.model = mesh.pos * CHUNK_SIZE;

        auto uploadSubMesh = [](const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds,
                                uint32_t& vao, uint32_t& vbo, uint32_t& ebo, uint32_t& count) {
            if (verts.empty()) return;

            count = static_cast<uint32_t>(inds.size());
            glCreateVertexArrays(1, &vao);
            glCreateBuffers(1, &vbo);
            glCreateBuffers(1, &ebo);

            glNamedBufferStorage(vbo, static_cast<GLsizeiptr>(verts.size() * sizeof(Vertex)), verts.data(), 0);
            glNamedBufferStorage(ebo, static_cast<GLsizeiptr>(inds.size() * sizeof(uint32_t)), inds.data(), 0);

            glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
            glVertexArrayElementBuffer(vao, ebo);

            glEnableVertexArrayAttrib(vao, 0);
            glEnableVertexArrayAttrib(vao, 1);
            glEnableVertexArrayAttrib(vao, 2);
            glEnableVertexArrayAttrib(vao, 3);
            glEnableVertexArrayAttrib(vao, 4);

            glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
            glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
            glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture));
            glVertexArrayAttribFormat(vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
            glVertexArrayAttribFormat(vao, 4, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, frames));

            glVertexArrayAttribBinding(vao, 0, 0);
            glVertexArrayAttribBinding(vao, 1, 0);
            glVertexArrayAttribBinding(vao, 2, 0);
            glVertexArrayAttribBinding(vao, 3, 0);
            glVertexArrayAttribBinding(vao, 4, 0);
        };

        uploadSubMesh(mesh.solidVertices, mesh.solidIndices, r.solidVao, r.solidVbo, r.solidEbo, r.solidCount);
        uploadSubMesh(mesh.transVertices, mesh.transIndices, r.transVao, r.transVbo, r.transEbo, r.transCount);

        m_renderables[mesh.pos] = r;
    }

    void Renderer::remove(const glm::ivec3& pos) {
        std::lock_guard lock(m_trashMutex);
        m_removed.push_back(pos);
    }

}
