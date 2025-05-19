#include "cube/graphics/VoxelRenderer.hpp"

#include <algorithm>
#include <unordered_set>
#include <glm/gtc/matrix_transform.hpp>

#include "cube/core/Constants.hpp"
#include "cube/graphics/Mesher.hpp"
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
        m_proj = glm::perspective(glm::radians(45.f),aspect,0.01f, (RENDER_DIST * CHUNK_WIDTH) * 2.f);
    }

    void VoxelRenderer::onDraw(const glm::mat4& view, const glm::vec3& root) {
        glEnable(GL_DEPTH);
        glEnable(GL_CULL_FACE);
        m_atlas.bind(0);

        m_shader.use();
        m_shader.setInt("atlas",0);
        m_shader.setMat4("proj", m_proj);
        m_shader.setMat4("view", view);

        std::vector<std::pair<glm::ivec2, VoxelItem>> chunks{m_items.begin(),m_items.end()};
        std::ranges::sort(chunks,[&](const auto& a, const auto& b) {
            const float distA = glm::distance(glm::vec3(a.first.x,root.y,a.first.y), root);
            const float distB = glm::distance(glm::vec3(b.first.x,root.y,b.first.y), root);
            return distA < distB;
        });

        for (const auto&[offset, item] : chunks) {
            m_shader.setMat4("model", glm::translate({1.f},glm::vec3{offset.x * CHUNK_WIDTH,0,offset.y * CHUNK_DEPTH}));
            glBindVertexArray(item.vao);
            glDrawElements(GL_TRIANGLES,item.count,GL_UNSIGNED_INT,nullptr);
        }
        glDisable(GL_DEPTH);
        glDisable(GL_CULL_FACE);
    }

    void VoxelRenderer::onTick(ThreadPool& pool, World& world) {
        std::unordered_set<glm::ivec2> current;

        world.forChunk([&](const glm::ivec2& chunk_pos) {;
            current.insert(chunk_pos);
            if (!m_items.contains(chunk_pos)) {
                pool.submit([this, chunk_pos, &world] {
                    if (auto mesh = m_mesher.toMesh(world,chunk_pos); !mesh.vertices.empty()) {
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
}
