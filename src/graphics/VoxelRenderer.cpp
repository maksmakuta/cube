#include "cube/graphics/VoxelRenderer.hpp"

#include <unordered_set>
#include <glm/gtc/matrix_transform.hpp>

#include "cube/core/Constants.hpp"
#include "cube/utils/Utils.hpp"
#include "glad/gl.h"

namespace cube {
    void VoxelItem::load() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    void VoxelItem::upload(const std::vector<Vertex3D> &vertices, const std::vector<unsigned> &indices) {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex3D)), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void *>(offsetof(Vertex3D, tex)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        count = static_cast<int>(indices.size());
    }

    void VoxelItem::unload() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    VoxelRenderer::VoxelRenderer() = default;
    VoxelRenderer::~VoxelRenderer() = default;

    void VoxelRenderer::onCreate() {
        m_shader.load(
            getAsset("/shaders/cube.vert"),
            getAsset("/shaders/cube.frag")
        );
        m_atlas.load(getAsset("/textures/atlas.png"));
        m_atlas.setWrap(TextureWrap::Repeat);
        m_atlas.setMagFilter(TextureFilter::Nearest);
        m_atlas.setMinFilter(TextureFilter::Nearest);
    }

    void VoxelRenderer::onClear() {
        m_shader.unload();
        m_atlas.unload();
    }

    void VoxelRenderer::onResize(const int w, const int h) {
        const auto aspect = static_cast<float>(w) / static_cast<float>(h);
        m_proj = glm::perspective(glm::radians(90.f),aspect,0.01f, (RENDER_DIST * CHUNK_WIDTH) * 2.f);
    }

    void VoxelRenderer::onDraw(const glm::mat4& view) {
        glEnable(GL_DEPTH);
        glEnable(GL_CULL_FACE);
        m_atlas.bind(0);

        m_shader.use();
        m_shader.setInt("atlas",0);
        m_shader.setMat4("proj", m_proj);
        m_shader.setMat4("view", view);

        for (const auto&[offset, item] : m_items) {
            m_shader.setMat4("model", glm::translate({1.f},glm::vec3{offset.x * CHUNK_WIDTH,0,offset.y * CHUNK_DEPTH}));
            glBindVertexArray(item.vao);
            glDrawElements(GL_TRIANGLES,item.count,GL_UNSIGNED_INT,nullptr);
        }
        glDisable(GL_DEPTH);
        glDisable(GL_CULL_FACE);

    }

    void VoxelRenderer::onTick(ThreadPool& pool, World& world) {
        std::unordered_set<glm::ivec2> current;

        world.forChunk([&](Chunk& chunk) {
            const auto pos = chunk.getOffset();
            current.insert(pos);
            if (!m_items.contains(pos)) {
                pool.submit([this, chunk] {
                    auto mesh = toMesh(chunk);
                    if (!mesh.vertices.empty()) {
                        std::lock_guard lock(m_meshes_mutex);
                        m_meshes.push_back(std::move(mesh));
                    }
                });
            }
        });

        {
            std::lock_guard lock(m_meshes_mutex);
            for (auto& [key, vertices, indices] : m_meshes) {
                auto rc = VoxelItem();
                rc.load();
                rc.upload(vertices, indices);
                m_items[key] = rc;
            }
            m_meshes.clear();
        }

        for (auto it = m_items.begin(); it != m_items.end(); ) {
            if (!current.contains(it->first)) {
                it->second.unload();
                it = m_items.erase(it);
            } else ++it;
        }

    }

    void addFace(std::vector<Vertex3D>& verts, std::vector<uint32_t>& indices, const glm::ivec3& blockPos,const BlockID& id, const int face) {
        static const glm::vec3 faceVerts[6][4] = {
            { {0,1,0}, {1,1,0}, {1,1,1}, {0,1,1} },
            { {0,0,0}, {0,0,1}, {1,0,1}, {1,0,0} },
            { {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0} },
            { {0,0,1}, {0,1,1}, {1,1,1}, {1,0,1} },
            { {0,0,0}, {0,1,0}, {0,1,1}, {0,0,1} },
            { {1,0,0}, {1,0,1}, {1,1,1}, {1,1,0} }
        };

        static constexpr glm::vec2 faceUVs[4] = {
            {0, 0}, {1, 0}, {1, 1}, {0, 1}
        };

        const auto startIndex = static_cast<uint32_t>(verts.size());
        const auto uv = getTile(id,face);
        for (int i = 0; i < 4; ++i) {
            verts.push_back({
                glm::vec3(blockPos) + faceVerts[face][i],
                glm::mix(glm::vec2{uv.x,uv.y},glm::vec2{uv.x,uv.y},faceUVs[i])
            });
        }

        indices.insert(indices.end(), {
            startIndex + 0, startIndex + 1, startIndex + 2,
            startIndex + 2, startIndex + 3, startIndex + 0
        });
    }

    glm::ivec3 faceNormal(const int face) {
        switch (face) {
            case 0: return {  0,  1,  0 };
            case 1: return {  0, -1,  0 };
            case 2: return {  0,  0, -1 };
            case 3: return {  0,  0,  1 };
            case 4: return { -1,  0,  0 };
            case 5: return {  1,  0,  0 };
            default: return { 0, 0, 0 };
        }
    }

    bool isInside(const glm::ivec3& vec) {
        return  vec.x >= 0 && vec.x < CHUNK_WIDTH &&
                vec.y >= 0 && vec.y < CHUNK_HEIGHT &&
                vec.z >= 0 && vec.z < CHUNK_DEPTH;
    }

    VoxelMesh VoxelRenderer::toMesh(const Chunk& chunk) {
        VoxelMesh result;
        result.key = chunk.getOffset();

        for (int x = 0; x < CHUNK_WIDTH; ++x)
            for (int z = 0; z < CHUNK_DEPTH; ++z)
                for (int y = 0; y < CHUNK_HEIGHT; ++y){
                    if (chunk.get({x, y, z}) == BlockID::Air) continue;

                    glm::ivec3 pos = { x, y, z };

                    for (int face = 0; face < 6; ++face) {
                        const auto neighbor = pos + faceNormal(face);
                        if (isInside(neighbor)) {
                            const auto id = chunk.get(neighbor);
                            if (id != BlockID::Air)
                                addFace(result.vertices, result.indices, pos, id, face);
                        }
                    }
                }

        return result;
    }
}
