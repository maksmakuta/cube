#include "cube/graphics/ChunkRenderer.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "cube/utils/Utils.hpp"
#include "glad/gl.h"

namespace cube {

    ChunkRenderable::ChunkRenderable() : vao(0),vbo(0),ebo(0),count(0){}

    void ChunkRenderable::uploadMesh(const std::vector<Vertex3D>& vertexData, const std::vector<glm::uint>& indexData) {
        if (vertexData.empty() || indexData.empty())
            return;
        if (vao == 0) glGenVertexArrays(1, &vao);
        if (vbo == 0) glGenBuffers(1, &vbo);
        if (ebo == 0) glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
                     vertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(uint32_t),
                     indexData.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Vertex3D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(float), reinterpret_cast<void *>(offsetof(Vertex3D, tex)));

        glBindVertexArray(0);
        count = static_cast<GLsizei>(indexData.size());
    }

    void ChunkRenderable::unload() {
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        if (vbo != 0) glDeleteBuffers(1, &vbo);
        if (ebo != 0) glDeleteBuffers(1, &ebo);
        vao = vbo = ebo = count = 0;
    }


    ChunkRenderer::ChunkRenderer() = default;
    ChunkRenderer::~ChunkRenderer() = default;

    void ChunkRenderer::onCreate() {
        m_shader.load(
            getAsset("/shaders/cube.vert"),
            getAsset("/shaders/cube.frag")
        );
        m_atlas.load(getAsset("/textures/atlas.png"));
    }

    void ChunkRenderer::onClear() {
        m_shader.unload();
        m_atlas.unload();
    }

    void ChunkRenderer::onResize(const int w, const int h) {
        const auto aspect = static_cast<float>(w) / static_cast<float>(h);
        m_projection = glm::perspective(glm::radians(90.f),aspect, 0.01f,RENDER_DIST * CHUNK_DEPTH * 1.75f);
    }

    void ChunkRenderer::onDraw(const glm::mat4& view) {
        m_atlas.bind(0);
        m_shader.use();
        m_shader.setInt("image",0);
        m_shader.setMat4("proj",m_projection);
        m_shader.setMat4("view",view);
        for (const auto& [offset, renderable] : m_chunks) {
            if (renderable.vbo == 0 || renderable.count == 0)
                continue;
            m_shader.setMat4("model",glm::translate(glm::mat4{1.f},glm::vec3{offset.x * CHUNK_WIDTH, 0 , offset.y * CHUNK_DEPTH}));
            glBindVertexArray(renderable.vao);
            glDrawElements(GL_TRIANGLES,renderable.count,GL_UNSIGNED_INT,nullptr);
        }
    }

    void calcMesh(std::vector<Vertex3D> & vertices, std::vector<glm::uint> & indices, Chunk & chunk) {


        static const glm::ivec3 directions[6] = {
            { 1,  0,  0}, {-1,  0,  0},
            { 0,  1,  0}, { 0, -1,  0},
            { 0,  0,  1}, { 0,  0, -1}
        };

        static constexpr glm::u16vec2 faceUVs[4] = {
            {0, 0}, {0xFFFF, 0}, {0xFFFF, 0xFFFF}, {0, 0xFFFF}
        };

        static const glm::u16vec3 faceOffsets[6][4] = {
            // +X
            {{1,0,0}, {1,1,0}, {1,1,1}, {1,0,1}},
            // -X
            {{0,0,1}, {0,1,1}, {0,1,0}, {0,0,0}},
            // +Y
            {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}},
            // -Y
            {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}},
            // +Z
            {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}},
            // -Z
            {{0,1,0}, {1,1,0}, {1,0,0}, {0,0,0}},
        };

        uint32_t indexStart = 0;

        auto isInside = [](const int x, const int y, const int z) {
            return x >= 0 && x < CHUNK_WIDTH &&
                   y >= 0 && y < CHUNK_HEIGHT &&
                   z >= 0 && z < CHUNK_DEPTH;
        };

        for (uint16_t x = 0; x < CHUNK_WIDTH; ++x)
            for (uint16_t y = 0; y < CHUNK_HEIGHT; ++y)
                for (uint16_t z = 0; z < CHUNK_DEPTH; ++z) {
                    const auto block = chunk.at({x, y, z});
                    if (block == BlockID::Air) continue;

                    for (int face = 0; face < 6; ++face) {
                        glm::ivec3 dir = directions[face];
                        int nx = x + dir.x, ny = y + dir.y, nz = z + dir.z;

                        if (!isInside(nx, ny, nz) || chunk.at({nx, ny, nz}) == BlockID::Air) {
                            for (int i = 0; i < 4; ++i) {
                                Vertex3D vert{};
                                vert.pos = glm::u16vec3(x, y, z) + faceOffsets[face][i];
                                vert.tex = faceUVs[i];
                                vertices.push_back(vert);
                            }

                            indices.push_back(indexStart + 0);
                            indices.push_back(indexStart + 1);
                            indices.push_back(indexStart + 2);
                            indices.push_back(indexStart + 2);
                            indices.push_back(indexStart + 3);
                            indices.push_back(indexStart + 0);

                            indexStart += 4;
                        }
                    }
                }

    }

    void ChunkRenderer::onTick(ThreadPool &pool, World &world) {
        auto temp = std::unordered_map<glm::ivec2,ChunkRenderable>();
        world.forChunk([this,&pool, &temp](Chunk& chunk) {
            const auto key = glm::ivec2(chunk.getOffset());
            if (m_chunks.contains(key)) {
                temp[key] = m_chunks[key];
            }else {
                temp[key] = ChunkRenderable();
                pool.submit([&key, &chunk, this] {
                    auto mesh = ChunkMesh(key,{},{});
                    calcMesh(mesh.vertices,mesh.indices,chunk);
                    {
                        std::lock_guard lock(m_mesh_mutex);
                        m_meshes.push_back(mesh);
                    }
                });
            }
        });

        if (!m_meshes.empty()) {
            for (const auto&[pos, vertices, indices] : m_meshes) {
                if (!temp.contains(pos)) {
                    temp[pos] = ChunkRenderable();
                }
                temp[pos].uploadMesh(vertices,indices);
            }
            m_meshes.clear();
        }

        m_chunks = std::move(temp);
    }


}
