#include "cube/graphics/RenderSystem.hpp"

#include <fstream>
#include <print>

#include "spng.h"
#include "cube/graphics/Vertex.hpp"

namespace cube {
    RenderSystem::RenderSystem() {
        m_shader = std::make_unique<Shader>(
            "../assets/shaders/cube.vert",
            "../assets/shaders/cube.frag"
        );
        initTextureArray();
    }

    void RenderSystem::update(World& world) {
        // 1. Get all chunks currently loaded in the world
        for (auto& [pos, chunk] : world.getChunks()) {
            auto& mesh = m_meshes[pos];
            if (mesh.dirty) {
                // Pass the REAL chunk by reference
                buildMesh(pos, chunk, mesh);
            }
        }
        std::erase_if(m_meshes, [&](const auto& pair) {
            return world.getChunk(pair.first) == nullptr;
        });
    }


    void RenderSystem::render(World& world, const Camera& camera, const glm::mat4& projection) {
        m_shader->use();

        // Bind the Texture Array to unit 0
        glBindTextureUnit(0, m_textureArray);

        m_shader->setMat4("view", camera.getViewMatrix());
        m_shader->setMat4("projection", projection);

        int drawed = 0;
        for (auto& [pos, mesh] : m_meshes) {
            if (mesh.indices == 0) continue;
            drawed++;
            // Calculate world position based on chunk coordinate
            glm::mat4 model = glm::translate(glm::mat4(1.0f),
                glm::vec3(pos.x * CHUNK_SIZE, pos.y * CHUNK_SIZE, pos.z * CHUNK_SIZE));

            m_shader->setMat4("model", model);

            glBindVertexArray(mesh.vao);
            glDrawElements(GL_TRIANGLES, mesh.indices, GL_UNSIGNED_INT, nullptr);
        }

        std::println("Rendering {} meshes, drawed {}", m_meshes.size(), drawed);
    }

    struct ImageData {
        std::vector<unsigned char> pixels;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    ImageData load_png(const std::filesystem::path& path) {
        // 1. Read file into memory
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) throw std::runtime_error("Could not open file: " + path.string());

        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> fileData(size);
        file.read(fileData.data(), size);

        // 2. Create spng context
        spng_ctx* ctx = spng_ctx_new(0);
        std::unique_ptr<spng_ctx, decltype(&spng_ctx_free)> ctx_ptr(ctx, spng_ctx_free);

        spng_set_png_buffer(ctx, fileData.data(), size);

        // 3. Get image header
        spng_ihdr ihdr;
        spng_get_ihdr(ctx, &ihdr);

        // 4. Determine output size and decode
        size_t out_size;
        spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);

        ImageData result;
        result.width = ihdr.width;
        result.height = ihdr.height;
        result.pixels.resize(out_size);

        if (spng_decode_image(ctx, result.pixels.data(), out_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS)) {
            throw std::runtime_error("Failed to decode PNG: " + path.string());
        }

        return result;
    }

    void RenderSystem::initTextureArray() {
        // Note the order matches our getLayer() logic
        const std::vector<std::string> textureFiles = {
            "dirt.png",                    // Layer 0
            "stone.png",                   // Layer 1
            "grass_block_top.png",         // Layer 2
            "grass_block_side.png",        // Layer 3
            "grass_block_side_overlay.png" // Layer 4
        };

        const uint32_t texSize = 16;
        const uint32_t layers = static_cast<uint32_t>(textureFiles.size());

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_textureArray);

        // Allocate immutable storage for 5 layers of 16x16 RGBA8
        glTextureStorage3D(m_textureArray, 1, GL_RGBA8, texSize, texSize, layers);

        for (uint32_t i = 0; i < layers; ++i) {
            try {
                auto img = load_png("../assets/textures/" + textureFiles[i]);

                // Validate size to prevent GPU memory corruption
                if (img.width != texSize || img.height != texSize) {
                    std::println(stderr, "Warning: {} is not 16x16!", textureFiles[i]);
                }

                // Upload to the specific layer i
                glTextureSubImage3D(m_textureArray, 0, 0, 0, i, texSize, texSize, 1,
                                    GL_RGBA, GL_UNSIGNED_BYTE, img.pixels.data());
            } catch (const std::exception& e) {
                std::println(stderr, "Failed to load layer {}: {}", i, e.what());
            }
        }

        // CRITICAL: Nearest filtering for 16x16 textures
        glTextureParameteri(m_textureArray, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_textureArray, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Wrapping usually doesn't matter for voxels if UVs are 0-1, but Repeat is safe
        glTextureParameteri(m_textureArray, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_textureArray, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    enum class Face {
        Top,
        Bottom,
        Left,
        Right,
        Front,
        Back
    };

    float getLayer(Block type, Face face) {
        switch (type) {
            case Block::Grass:
                if (face == Face::Top)    return 2.0f; // grass_block_top.png
                if (face == Face::Bottom) return 0.0f; // dirt.png
                return 3.0f;                           // grass_block_side.png
            case Block::Dirt:  return 0.0f;            // dirt.png
            case Block::Stone: return 2.0f;            // stone.png
            default: return 0.0f;
        }
    }

    glm::vec3 getTint(Block type, Face face) {
        if (type == Block::Grass && face != Face::Bottom ) return {0.37f, 0.71f, 0.25f};

        return {1.0f, 1.0f, 1.0f};
    }

    void addFaceData(std::vector<Vertex>& verts, std::vector<uint32_t>& indices,
                  uint32_t& offset, int x, int y, int z, Face face, Block type) {
        float layer = getLayer(type, face);
        glm::vec3 tint = getTint(type, face);

        // Casting int coordinates to float once
        float fx = static_cast<float>(x);
        float fy = static_cast<float>(y);
        float fz = static_cast<float>(z);

        switch (face) {
            case Face::Top: // +Y
                verts.push_back({{fx,     fy + 1, fz},     {0, 1,layer}, tint}); // TL
                verts.push_back({{fx,     fy + 1, fz + 1}, {0, 0,layer}, tint}); // BL
                verts.push_back({{fx + 1, fy + 1, fz + 1}, {1, 0,layer}, tint}); // BR
                verts.push_back({{fx + 1, fy + 1, fz},     {1, 1,layer}, tint}); // TR
                break;

            case Face::Bottom: // -Y
                verts.push_back({{fx,     fy, fz},     {0, 0, layer}, tint});
                verts.push_back({{fx + 1, fy, fz},     {1, 0, layer}, tint});
                verts.push_back({{fx + 1, fy, fz + 1}, {1, 1, layer}, tint});
                verts.push_back({{fx,     fy, fz + 1}, {0, 1, layer}, tint});
                break;

            case Face::Front: // +Z
                verts.push_back({{fx,     fy,     fz + 1}, {0, 0,layer}, tint});
                verts.push_back({{fx + 1, fy,     fz + 1}, {1, 0,layer}, tint});
                verts.push_back({{fx + 1, fy + 1, fz + 1}, {1, 1,layer}, tint});
                verts.push_back({{fx,     fy + 1, fz + 1}, {0, 1,layer}, tint});
                break;

            case Face::Back: // -Z
                verts.push_back({{fx + 1, fy,     fz}, {0, 0,layer}, tint});
                verts.push_back({{fx,     fy,     fz}, {1, 0,layer}, tint});
                verts.push_back({{fx,     fy + 1, fz}, {1, 1,layer}, tint});
                verts.push_back({{fx + 1, fy + 1, fz}, {0, 1,layer}, tint});
                break;

            case Face::Left: // -X
                verts.push_back({{fx, fy,     fz},     {0, 0,layer}, tint});
                verts.push_back({{fx, fy,     fz + 1}, {1, 0,layer}, tint});
                verts.push_back({{fx, fy + 1, fz + 1}, {1, 1,layer}, tint});
                verts.push_back({{fx, fy + 1, fz},     {0, 1,layer}, tint});
                break;

            case Face::Right: // +X
                verts.push_back({{fx + 1, fy,     fz + 1}, {0, 0,layer}, tint});
                verts.push_back({{fx + 1, fy,     fz},     {1, 0,layer}, tint});
                verts.push_back({{fx + 1, fy + 1, fz},     {1, 1,layer}, tint});
                verts.push_back({{fx + 1, fy + 1, fz + 1}, {0, 1,layer}, tint});
                break;
        }

        // Standard Indexing for a Quad (Indices 0,1,2 and 2,3,0)
        // This assumes the vertices above were pushed in CCW order for each face
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 0);

        offset += 4;
    }

    void RenderSystem::buildMesh(const glm::ivec3 &pos, const Chunk &chunk, ChunkMesh &mesh) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // Reserve memory to avoid reallocations
        vertices.reserve(1000);
        indices.reserve(1500);

        auto view = chunk.getView();
        uint32_t vertexOffset = 0;

        uint32_t offset = 0;
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    Block block = view[x, y, z];
                    if (block == Block::Air) continue;

                    if (y == CHUNK_SIZE - 1 || view[x, y + 1, z] == Block::Air)
                        addFaceData(vertices, indices, offset, x, y, z, Face::Top, block);
                    if (y == 0 || view[x, y - 1, z] == Block::Air)
                        addFaceData(vertices, indices, offset, x, y, z, Face::Bottom, block);

                    // +Z / -Z
                    if (z == CHUNK_SIZE - 1 || view[x, y, z + 1] == Block::Air)
                        addFaceData(vertices, indices, offset, x, y, z, Face::Front, block);
                    if (z == 0 || view[x, y, z - 1] == Block::Air)
                        addFaceData(vertices, indices, offset, x, y, z, Face::Back, block);

                    // +X / -X
                    if (x == CHUNK_SIZE - 1 || view[x + 1, y, z] == Block::Air)
                        addFaceData(vertices, indices, offset, x, y, z, Face::Right, block);
                    if (x == 0 || view[x - 1, y, z] == Block::Air)
                        addFaceData(vertices, indices, offset, x, y, z, Face::Left, block);
                }
            }
        }

        if (vertices.empty()) {
            mesh.indices = 0;
            return;
        }

        // Initialize Buffers using DSA if not already created
        if (mesh.vao == 0) {
            glCreateVertexArrays(1, &mesh.vao);
            glCreateBuffers(1, &mesh.vbo);
            glCreateBuffers(1, &mesh.ebo);

            // Setup Attributes (one-time setup)
            glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(Vertex));
            glVertexArrayElementBuffer(mesh.vao, mesh.ebo);

            glEnableVertexArrayAttrib(mesh.vao, 0); // Pos
            glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
            glVertexArrayAttribBinding(mesh.vao, 0, 0);

            glEnableVertexArrayAttrib(mesh.vao, 1); // UV
            glVertexArrayAttribFormat(mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
            glVertexArrayAttribBinding(mesh.vao, 1, 0);

            glEnableVertexArrayAttrib(mesh.vao, 2); // Tint
            glVertexArrayAttribFormat(mesh.vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tint));
            glVertexArrayAttribBinding(mesh.vao, 2, 0);
        }

        // Upload Data
        glNamedBufferData(mesh.vbo, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glNamedBufferData(mesh.ebo, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        mesh.indices = static_cast<GLsizei>(indices.size());
        mesh.dirty = false;
    }


}
