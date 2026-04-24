#include <cube/graphics/Renderer.hpp>
#include <cube/utils/Logger.hpp>

#include <expected>
#include <fstream>
#include <ranges>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <spng.h>

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

    static std::expected<GLuint, std::string> createProgram(const char* vertPath, const char* fragPath) {
        auto vertSrc = readFile(vertPath);
        auto fragSrc = readFile(fragPath);

        if (!vertSrc) return std::unexpected(vertSrc.error());
        if (!fragSrc) return std::unexpected(fragSrc.error());

        auto vShader = compileShader(GL_VERTEX_SHADER, vertSrc->c_str());
        if (!vShader) return vShader;

        auto fShader = compileShader(GL_FRAGMENT_SHADER, fragSrc->c_str());
        if (!fShader) return fShader;

        GLuint program = glCreateProgram();
        glAttachShader(program, *vShader);
        glAttachShader(program, *fShader);
        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            return std::unexpected(std::string("Linking Error: ") + infoLog);
        }

        glDeleteShader(*vShader);
        glDeleteShader(*fShader);

        return program;
    }

    void Renderer::loadShader() {
        if (const auto shader = createProgram(
            "../assets/shaders/cube.vert",
            "../assets/shaders/cube.frag"
        )) {
            m_shader = *shader;
        } else {
            Log::error("Cannot create shader program: {}", shader.error());
            m_shader = 0;
        }
    }

    void Renderer::loadTextures() {
        const std::vector<std::string> textures = {
            "grass_block_side_overlay.png",
            "grass_block_top.png",
            "grass_block_side.png",
            "dirt.png",
            "stone.png",
            "sand.png"
        };

        constexpr int width = 16;
        constexpr int height = 16;

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);
        glTextureStorage3D(m_texture, 1, GL_RGBA8, width, height, static_cast<GLsizei>(textures.size()));

        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        for (size_t i = 0; i < textures.size(); ++i) {
            const std::string path = std::format("../assets/textures/{}", textures[i]);
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file) {
                Log::error( "Failed to open texture: {}", path);
                continue;
            }

            size_t fileSize = file.tellg();
            std::vector<char> fileBuffer(fileSize);
            file.seekg(0);
            file.read(fileBuffer.data(), fileSize);

            // 2. Setup spng context
            spng_ctx* ctx = spng_ctx_new(0);
            spng_set_png_buffer(ctx, fileBuffer.data(), fileSize);

            size_t out_size;
            spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);

            std::vector<unsigned char> rgbaBuffer(out_size);

            if (const int ret = spng_decode_image(ctx, rgbaBuffer.data(), out_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS)) {
                Log::error( "spng error: {}", spng_strerror(ret));
                spng_ctx_free(ctx);
                continue;
            }

            glTextureSubImage3D(m_texture,
                0,
                0, 0, static_cast<GLint>(i),
                width, height, 1,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                rgbaBuffer.data()
            );

            spng_ctx_free(ctx);
            Log::info("Loaded layer {}: {}", i, textures[i]);
        }
    }

    void Renderer::render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos) {
        glUseProgram(m_shader);

        glUniformMatrix4fv(glGetUniformLocation(m_shader, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, GL_FALSE, &view[0][0]);

        glBindTextureUnit(0, m_texture);

        std::vector<glm::ivec3> sortedRenderables;
        for (const auto &pos: m_renderables | std::views::keys) {
            sortedRenderables.push_back(pos);
        }

        std::sort(sortedRenderables.begin(), sortedRenderables.end(),
            [&cameraPos](const glm::ivec3& a, const glm::ivec3& b) {
                const float distA = glm::distance(cameraPos,glm::vec3(a * 16));
                const float distB = glm::distance(cameraPos,glm::vec3(b * 16));
                return distA < distB;
        });

        // 3. Render in sorted order
        for (const auto& r : sortedRenderables) {
            if (!m_renderables.contains(r)) {
                continue;
            }
            const auto renderable = m_renderables[r];
            glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, &renderable.model[0][0]);
            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(renderable.count), GL_UNSIGNED_INT, nullptr);
        }
    }

    void Renderer::put(const glm::ivec3& pos, const RenderableMesh& mesh) {
        if (mesh.vertices.empty()) return;

        Renderable r{};
        r.count = static_cast<uint32_t>(mesh.indices.size());

        glCreateVertexArrays(1, &r.vao);

        glCreateBuffers(1, &r.vbo);
        glCreateBuffers(1, &r.ebo);

        glNamedBufferStorage(r.vbo, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), 0);
        glNamedBufferStorage(r.ebo, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), 0);

        glVertexArrayVertexBuffer(r.vao, 0, r.vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(r.vao, r.ebo);

        glEnableVertexArrayAttrib(r.vao, 0);
        glEnableVertexArrayAttrib(r.vao, 1);
        glEnableVertexArrayAttrib(r.vao, 2);

        glVertexArrayAttribFormat(r.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribFormat(r.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glVertexArrayAttribFormat(r.vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture));

        glVertexArrayAttribBinding(r.vao, 0, 0);
        glVertexArrayAttribBinding(r.vao, 1, 0);
        glVertexArrayAttribBinding(r.vao, 2, 0);

        r.model = glm::translate(glm::mat4(1.0f), glm::vec3(pos * 16));

        m_renderables[pos] = r;
    }

}
