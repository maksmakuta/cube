#include "cube/graphics/VoxelRenderer.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <glm/gtc/matrix_transform.hpp>

#include "cube/core/Constants.hpp"
#include "cube/graphics/Mesher.hpp"
#include "cube/utils/Utils.hpp"
#include "glad/gl.h"

namespace cube {

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
        glEnable(GL_DEPTH_TEST);
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
        m_atlas.bind(0);

        m_shader.use();
        m_shader.setInt("atlas",0);
        m_shader.setMat4("proj", m_proj);
        m_shader.setMat4("view", view);

        {
            std::shared_lock lock(m_mesh_mutex);
            std::vector<std::pair<glm::ivec2, VoxelItem>> chunks{m_items.begin(),m_items.end()};
            std::ranges::sort(chunks,[&](const auto& a, const auto& b) {
                const float distA = glm::distance(glm::vec3(a.first.x,0,a.first.y), glm::vec3(root.x,0,root.z));
                const float distB = glm::distance(glm::vec3(b.first.x,0,b.first.y), glm::vec3(root.x,0,root.z));
                return distA > distB;
            });

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);

            for (const auto&[offset, item] : chunks) {
                m_shader.setMat4("model", glm::translate({1.f},glm::vec3{offset.x * CHUNK_WIDTH,0,offset.y * CHUNK_DEPTH}));
                glBindVertexArray(item.terrain.vao);
                glDrawElements(GL_TRIANGLES, item.terrain.count,GL_UNSIGNED_INT, nullptr);
            }

            glDisable(GL_CULL_FACE);
            glDepthMask(GL_FALSE);

            for (const auto &[offset, item]: chunks) {
                m_shader.setMat4("model", glm::translate({1.f}, glm::vec3{
                                                             offset.x * CHUNK_WIDTH, 0, offset.y * CHUNK_DEPTH
                                                         }));
                glBindVertexArray(item.billboard.vao);
                glDrawElements(GL_TRIANGLES, item.billboard.count,GL_UNSIGNED_INT, nullptr);
            }
            glDepthMask(GL_TRUE);
        }
    }

    void VoxelRenderer::onTick(ThreadPool& pool, World& world) {
        std::unordered_set<glm::ivec2> active_chunks;

        glm::ivec2 r_min{0};
        glm::ivec2 r_max{0};

        world.forChunk([this, &active_chunks, &pool, &world, &r_min, &r_max](const ChunkPtr& ptr, const glm::ivec2& pos) {

            r_min.x = std::min(r_min.x,pos.x);
            r_min.y = std::min(r_min.y,pos.y);
            r_max.x = std::max(r_min.x,pos.x);
            r_max.y = std::max(r_min.y,pos.y);

            active_chunks.insert(pos);
            if (!m_items.contains(pos) || isBorder(pos)) {
                pool.submit([this, &ptr, &pos, &world] {
                    /*      Y+
                     *    0 1 2
                     * X- 3 x 4 X+
                     *    5 6 7
                     *      Y-
                     */

                    auto neighbors = std::array<ChunkPtr, 8>();
                    neighbors[0] = world.at(pos + glm::ivec2{-1, 1});
                    neighbors[1] = world.at(pos + glm::ivec2{0, 1});
                    neighbors[2] = world.at(pos + glm::ivec2{1, 1});
                    neighbors[3] = world.at(pos + glm::ivec2{-1, 0});
                    neighbors[4] = world.at(pos + glm::ivec2{1, 0});
                    neighbors[5] = world.at(pos + glm::ivec2{-1, -1});
                    neighbors[6] = world.at(pos + glm::ivec2{0, -1});
                    neighbors[7] = world.at(pos + glm::ivec2{1, -1});
                    const auto mesh = m_mesher.toMesh(ptr,neighbors,pos);
                    std::lock_guard lock(m_mesh_mutex);
                    m_meshes.push_back(mesh);
                });
            }
        });

        if (!m_items.empty()){
            std::lock_guard lock(m_mesh_mutex);
            for (auto it = m_items.begin(); it != m_items.end();) {
                if (!active_chunks.contains(it->first)) {
                    it->second.terrain.unload();
                    it->second.billboard.unload();
                    it = m_items.erase(it);
                }else {
                    ++it;
                }
            }
        }

        if (!m_meshes.empty()) {
            std::lock_guard lock(m_mesh_mutex);
            for (const auto &[key, terrain, billboard]: m_meshes) {
                if (m_items.contains(key)) {
                    auto &i = m_items[key];
                    i.terrain.upload(terrain.vertices, terrain.indices);
                    i.billboard.upload(billboard.vertices, billboard.indices);
                } else {
                    auto item = VoxelItem();
                    item.terrain.load();
                    item.billboard.load();

                    item.terrain.upload(terrain.vertices, terrain.indices);
                    item.billboard.upload(billboard.vertices, billboard.indices);
                    m_items[key] = item;
                }

            }
            m_meshes.clear();
        }

        range = {r_min,r_max};

    }

    bool VoxelRenderer::isBorder(const glm::ivec2& v) const {
        return v.x == range.x || v.y == range.y || v.x == range.z || v.y == range.w;
    }
}
