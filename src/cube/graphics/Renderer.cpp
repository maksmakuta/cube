#include "cube/graphics/Renderer.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>

#include <glad/glad.h>

#include "spng.h"
#include "cube/core/Config.hpp"

#ifndef ASSETS_DIR
#error "ASSETS_DIR not defined"
#endif

namespace cube {

    const auto ASSETS = std::string(ASSETS_DIR);

    Renderer::Renderer() {
        loadShaders();
        loadTextures();
    }

    Renderer::~Renderer() {
        for (auto &obj: m_objects | std::views::values) {
            if (obj.solid_vao) glDeleteVertexArrays(1, &obj.solid_vao);
            if (obj.solid_vbo) glDeleteBuffers(1, &obj.solid_vbo);
            if (obj.solid_ebo) glDeleteBuffers(1, &obj.solid_ebo);

            if (obj.blend_vao) glDeleteVertexArrays(1, &obj.blend_vao);
            if (obj.blend_vbo) glDeleteBuffers(1, &obj.blend_vbo);
            if (obj.blend_ebo) glDeleteBuffers(1, &obj.blend_ebo);
        }
    }

    void Renderer::uploadChunk(const glm::ivec3& pos, const Mesh& mesh) {
        if (const auto it = m_objects.find(pos); it != m_objects.end()) {
            const GLObject& oldObj = it->second;
            if (oldObj.solid_vao) glDeleteVertexArrays(1, &oldObj.solid_vao);
            if (oldObj.solid_vbo) glDeleteBuffers(1, &oldObj.solid_vbo);
            if (oldObj.solid_ebo) glDeleteBuffers(1, &oldObj.solid_ebo);
            if (oldObj.blend_vao) glDeleteVertexArrays(1, &oldObj.blend_vao);
            if (oldObj.blend_vbo) glDeleteBuffers(1, &oldObj.blend_vbo);
            if (oldObj.blend_ebo) glDeleteBuffers(1, &oldObj.blend_ebo);
        }

        GLObject newObj{};

        if (!mesh.solid.vertices.empty()) {
            glGenVertexArrays(1, &newObj.solid_vao);
            glGenBuffers(1, &newObj.solid_vbo);
            glGenBuffers(1, &newObj.solid_ebo);

            glBindVertexArray(newObj.solid_vao);
            glBindBuffer(GL_ARRAY_BUFFER, newObj.solid_vbo);
            glBufferData(GL_ARRAY_BUFFER, mesh.solid.vertices.size() * sizeof(Vertex), mesh.solid.vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObj.solid_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.solid.indices.size() * sizeof(uint32_t), mesh.solid.indices.data(), GL_STATIC_DRAW);

            glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, 0, static_cast<void *>(nullptr));
            glEnableVertexAttribArray(0);

            newObj.solid_count = static_cast<uint32_t>(mesh.solid.indices.size());
        }

        if (!mesh.transparent.vertices.empty()) {
            glGenVertexArrays(1, &newObj.blend_vao);
            glGenBuffers(1, &newObj.blend_vbo);
            glGenBuffers(1, &newObj.blend_ebo);

            glBindVertexArray(newObj.blend_vao);
            glBindBuffer(GL_ARRAY_BUFFER, newObj.blend_vbo);
            glBufferData(GL_ARRAY_BUFFER, mesh.transparent.vertices.size() * sizeof(Vertex), mesh.transparent.vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObj.blend_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.transparent.indices.size() * sizeof(uint32_t), mesh.transparent.indices.data(), GL_STATIC_DRAW);

            glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, 0, static_cast<void *>(nullptr));
            glEnableVertexAttribArray(0);

            newObj.blend_count = static_cast<uint32_t>(mesh.transparent.indices.size());
        }

        m_objects[pos] = newObj;

        if (std::ranges::find(m_list, pos) == m_list.end()) {
            m_list.push_back(pos);
        }
    }

    void Renderer::unloadChunks(const glm::ivec3& pos, const int render_distance) {
        auto it = m_objects.begin();
        while (it != m_objects.end()) {
            glm::ivec3 chunkPos = it->first;

            const auto distance = static_cast<int>(glm::floor(glm::length(glm::vec3(chunkPos - pos))));

            if (distance > render_distance) {
                GLObject& obj = it->second;
                if (obj.solid_vao) glDeleteVertexArrays(1, &obj.solid_vao);
                if (obj.solid_vbo) glDeleteBuffers(1, &obj.solid_vbo);
                if (obj.solid_ebo) glDeleteBuffers(1, &obj.solid_ebo);
                if (obj.blend_vao) glDeleteVertexArrays(1, &obj.blend_vao);
                if (obj.blend_vbo) glDeleteBuffers(1, &obj.blend_vbo);
                if (obj.blend_ebo) glDeleteBuffers(1, &obj.blend_ebo);

                m_list.erase(std::ranges::remove(m_list, chunkPos).begin(), m_list.end());

                it = m_objects.erase(it);
            } else {
                ++it;
            }
        }
    }

    void Renderer::render(const Camera& camera) {
        m_shader.use();
        m_shader.setMat4("u_Projection", camera.getProjection());
        m_shader.setMat4("u_View", camera.getViewMatrix());

        glm::vec3 cameraPos = camera.getPosition();

        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
        glActiveTexture(GL_TEXTURE0);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        for (const auto& pos : m_list) {
            if (const GLObject& obj = m_objects[pos]; obj.solid_count > 0) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos * CHUNK_SIZE));
                m_shader.setMat4("u_Model", model);

                glBindVertexArray(obj.solid_vao);
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(obj.solid_count), GL_UNSIGNED_INT, nullptr);
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        std::ranges::sort(m_list, [&cameraPos](const glm::ivec3& a, const glm::ivec3& b) {
            const float distA = glm::distance(cameraPos, glm::vec3(a * CHUNK_SIZE + 8));
            const float distB = glm::distance(cameraPos, glm::vec3(b * CHUNK_SIZE + 8));
            return distA > distB;
        });

        for (const auto& pos : m_list) {
            if (const GLObject& obj = m_objects[pos]; obj.blend_count > 0) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos * CHUNK_SIZE));
                m_shader.setMat4("u_Model", model);

                glBindVertexArray(obj.blend_vao);
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(obj.blend_count), GL_UNSIGNED_INT, nullptr);
            }
        }

        glDepthMask(GL_TRUE);
    }

    std::string readFile(const std::string& path) {
        if (std::ifstream ifs(path); ifs.is_open()) {
            return (std::stringstream() << ifs.rdbuf()).str();
        }
        return "";
    }

    void Renderer::loadShaders() {
        const auto vert = readFile(ASSETS + "/shaders/voxel.vert");
        const auto frag = readFile(ASSETS + "/shaders/voxel.frag");
        m_shader.compile(vert, frag);
    }

    struct TextureInfo {
        std::string path;
        int frames;
    };














    const auto textures = std::vector<TextureInfo>{
        {"textures/brown_mushroom.png",                             1   },
        {"textures/melon_side.png",                                 1   },
        {"textures/melon_top.png",                                  1   },
        {"textures/pumpkin_side.png",                               1   },
        {"textures/pumpkin_top.png",                                1   },
        {"textures/red_mushroom.png",                               1   },
        {"textures/cactus/cactus_bottom.png",                       1   },
        {"textures/cactus/cactus_flower.png",                       1   },
        {"textures/cactus/cactus_side.png",                         1   },
        {"textures/cactus/cactus_top.png",                          1   },
        {"textures/coral/block/brain_coral_block.png",              1   },
        {"textures/coral/block/bubble_coral_block.png",             1   },
        {"textures/coral/block/dead_brain_coral_block.png",         1   },
        {"textures/coral/block/dead_bubble_coral_block.png",        1   },
        {"textures/coral/block/dead_fire_coral_block.png",          1   },
        {"textures/coral/block/dead_horn_coral_block.png",          1   },
        {"textures/coral/block/dead_tube_coral_block.png",          1   },
        {"textures/coral/block/fire_coral_block.png",               1   },
        {"textures/coral/block/horn_coral_block.png",               1   },
        {"textures/coral/block/tube_coral_block.png",               1   },
        {"textures/coral/brain_coral.png",                          1   },
        {"textures/coral/bubble_coral.png",                         1   },
        {"textures/coral/dead_brain_coral.png",                     1   },
        {"textures/coral/dead_bubble_coral.png",                    1   },
        {"textures/coral/dead_fire_coral.png",                      1   },
        {"textures/coral/dead_horn_coral.png",                      1   },
        {"textures/coral/dead_tube_coral.png",                      1   },
        {"textures/coral/fan/brain_coral_fan.png",                  1   },
        {"textures/coral/fan/bubble_coral_fan.png",                 1   },
        {"textures/coral/fan/dead_brain_coral_fan.png",             1   },
        {"textures/coral/fan/dead_bubble_coral_fan.png",            1   },
        {"textures/coral/fan/dead_fire_coral_fan.png",              1   },
        {"textures/coral/fan/dead_horn_coral_fan.png",              1   },
        {"textures/coral/fan/dead_tube_coral_fan.png",              1   },
        {"textures/coral/fan/fire_coral_fan.png",                   1   },
        {"textures/coral/fan/horn_coral_fan.png",                   1   },
        {"textures/coral/fan/tube_coral_fan.png",                   1   },
        {"textures/coral/fire_coral.png",                           1   },
        {"textures/coral/horn_coral.png",                           1   },
        {"textures/coral/tube_coral.png",                           1   },
        {"textures/decorations/bush.png",                           1   },
        {"textures/decorations/cave_vines_lit.png",                 1   },
        {"textures/decorations/cave_vines_plant_lit.png",           1   },
        {"textures/decorations/cave_vines_plant.png",               1   },
        {"textures/decorations/cave_vines.png",                     1   },
        {"textures/decorations/dead_bush.png",                      1   },
        {"textures/decorations/fern.png",                           1   },
        {"textures/decorations/firefly_bush.png",                   1   },
        {"textures/decorations/grass.png",                          1   },
        {"textures/decorations/hanging_roots.png",                  1   },
        {"textures/decorations/short_dry_grass.png",                1   },
        {"textures/decorations/short_grass.png",                    1   },
        {"textures/decorations/sugar_cane.png",                     1   },
        {"textures/decorations/sweet_berry_bush_stage0.png",        1   },
        {"textures/decorations/sweet_berry_bush_stage1.png",        1   },
        {"textures/decorations/sweet_berry_bush_stage2.png",        1   },
        {"textures/decorations/sweet_berry_bush_stage3.png",        1   },
        {"textures/decorations/tall_dry_grass.png",                 1   },
        {"textures/flowers/allium.png",                             1   },
        {"textures/flowers/azure_bluet.png",                        1   },
        {"textures/flowers/blue_orchid.png",                        1   },
        {"textures/flowers/cornflower.png",                         1   },
        {"textures/flowers/dandelion.png",                          1   },
        {"textures/flowers/orange_tulip.png",                       1   },
        {"textures/flowers/oxeye_daisy.png",                        1   },
        {"textures/flowers/pink_tulip.png",                         1   },
        {"textures/flowers/poppy.png",                              1   },
        {"textures/flowers/red_tulip.png",                          1   },
        {"textures/flowers/white_tulip.png",                        1   },
        {"textures/leaves/acacia_leaves.png",                       1   },
        {"textures/leaves/azalea_leaves.png",                       1   },
        {"textures/leaves/birch_leaves.png",                        1   },
        {"textures/leaves/cherry_leaves.png",                       1   },
        {"textures/leaves/dark_oak_leaves.png",                     1   },
        {"textures/leaves/jungle_leaves.png",                       1   },
        {"textures/leaves/oak_leaves.png",                          1   },
        {"textures/leaves/pale_oak_leaves.png",                     1   },
        {"textures/leaves/spruce_leaves.png",                       1   },
        {"textures/logs/acacia_log.png",                            1   },
        {"textures/logs/acacia_log_top.png",                        1   },
        {"textures/logs/birch_log.png",                             1   },
        {"textures/logs/birch_log_top.png",                         1   },
        {"textures/logs/cherry_log.png",                            1   },
        {"textures/logs/cherry_log_top.png",                        1   },
        {"textures/logs/dark_oak_log.png",                          1   },
        {"textures/logs/dark_oak_log_top.png",                      1   },
        {"textures/logs/jungle_log.png",                            1   },
        {"textures/logs/jungle_log_top.png",                        1   },
        {"textures/logs/oak_log.png",                               1   },
        {"textures/logs/oak_log_top.png",                           1   },
        {"textures/logs/pale_oak_log.png",                          1   },
        {"textures/logs/pale_oak_log_top.png",                      1   },
        {"textures/logs/spruce_log.png",                            1   },
        {"textures/logs/spruce_log_top.png",                        1   },
        {"textures/natural/amethyst_block.png",                     1   },
        {"textures/natural/andesite.png",                           1   },
        {"textures/natural/basalt_side.png",                        1   },
        {"textures/natural/basalt_top.png",                         1   },
        {"textures/natural/bedrock.png",                            1   },
        {"textures/natural/blackstone.png",                         1   },
        {"textures/natural/blackstone_top.png",                     1   },
        {"textures/natural/blue_ice.png",                           1   },
        {"textures/natural/calcite.png",                            1   },
        {"textures/natural/clay.png",                               1   },
        {"textures/natural/coarse_dirt.png",                        1   },
        {"textures/natural/cobblestone.png",                        1   },
        {"textures/natural/deepslate.png",                          1   },
        {"textures/natural/deepslate_top.png",                      1   },
        {"textures/natural/diorite.png",                            1   },
        {"textures/natural/dirt.png",                               1   },
        {"textures/natural/dripstone_block.png",                    1   },
        {"textures/natural/granite.png",                            1   },
        {"textures/natural/grass_block_side_overlay.png",           1   },
        {"textures/natural/grass_block_side.png",                   1   },
        {"textures/natural/grass_block_snow.png",                   1   },
        {"textures/natural/grass_block_top.png",                    1   },
        {"textures/natural/gravel.png",                             1   },
        {"textures/natural/ice.png",                                1   },
        {"textures/natural/mud.png",                                1   },
        {"textures/natural/packed_ice.png",                         1   },
        {"textures/natural/red_sand.png",                           1   },
        {"textures/natural/red_sandstone.png",                      1   },
        {"textures/natural/sand.png",                               1   },
        {"textures/natural/sandstone.png",                          1   },
        {"textures/natural/stone.png",                              1   },
        {"textures/natural/tuff.png",                               1   },
        {"textures/sapling/acacia_sapling.png",                     1   },
        {"textures/sapling/azalea_plant.png",                       1   },
        {"textures/sapling/azalea_side.png",                        1   },
        {"textures/sapling/azalea_top.png",                         1   },
        {"textures/sapling/birch_sapling.png",                      1   },
        {"textures/sapling/cherry_sapling.png",                     1   },
        {"textures/sapling/dark_oak_sapling.png",                   1   },
        {"textures/sapling/flowering_azalea_leaves.png",            1   },
        {"textures/sapling/flowering_azalea_side.png",              1   },
        {"textures/sapling/flowering_azalea_top.png",               1   },
        {"textures/sapling/jungle_sapling.png",                     1   },
        {"textures/sapling/oak_sapling.png",                        1   },
        {"textures/sapling/pale_oak_sapling.png",                   1   },
        {"textures/sapling/spruce_sapling.png",                     1   },
        {"textures/tall/large_fern_bottom.png",                     1   },
        {"textures/tall/large_fern_top.png",                        1   },
        {"textures/tall/peony_bottom.png",                          1   },
        {"textures/tall/peony_top.png",                             1   },
        {"textures/tall/rose_bush_bottom.png",                      1   },
        {"textures/tall/rose_bush_top.png",                         1   },
        {"textures/tall/tall_grass_bottom.png",                     1   },
        {"textures/tall/tall_grass_top.png",                        1   },
        {"textures/tall/tall_seagrass_bottom.png",                  18  },
        {"textures/tall/tall_seagrass_top.png",                     18  },
        {"textures/kelp_plant.png",                                 32  },
        {"textures/lava_still.png",                                 32  },
        {"textures/seagrass.png",                                   32  },
        {"textures/water_still.png",                                32  },
};

    void Renderer::loadTextures(const glm::ivec2& tile_size) {
        int layers = 0;

        for (const TextureInfo& info : textures) {
            layers += info.frames;
        }

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);
        const int mipLevels = 1 + static_cast<int>(std::floor(std::log2(tile_size.x)));
        glTextureStorage3D(m_texture, mipLevels, GL_RGBA8, tile_size.x, tile_size.y, layers);

        int layer = 0;
        for (const auto&[path, frames] : textures) {
            const auto filepath = ASSETS + "/" += path;
            std::ifstream file(filepath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cerr << "Failed to open asset path: " << filepath << "\n";
                continue;
            }

            size_t fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<char> buffer(fileSize);
            file.read(buffer.data(), fileSize);

            spng_ctx* ctx = spng_ctx_new(0);
            if (!ctx) {
                continue;
            }

            if (spng_set_png_buffer(ctx, buffer.data(), fileSize)) {
                spng_ctx_free(ctx);
                continue;
            }

            size_t outSize;
            if (spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &outSize)) {
                spng_ctx_free(ctx);
                continue;
            }

            spng_ihdr ihdr{};
            spng_get_ihdr(ctx, &ihdr);

            std::vector<uint8_t> pixels(outSize);
            if (spng_decode_image(ctx, pixels.data(), outSize, SPNG_FMT_RGBA8, 0)) {
                std::cerr << "Failed to decode PNG format context for: " << path << "\n";
                spng_ctx_free(ctx);
                continue;
            }

            std::cout << "Loaded: " << path
                      << " | Base Layer: " << layer
                      << " | Frames: " << frames << "\n";

            if (frames <= 1) {
                glTextureSubImage3D(m_texture, 0, 0, 0, layer, tile_size.x, tile_size.y, 1,
                                    GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
                layer++;
            } else {
                const int strideBytes = tile_size.x * 4;
                const size_t frameSizeBytes = tile_size.y * strideBytes;
                std::vector<uint8_t> frameBuffer(frameSizeBytes);

                for (int f = 0; f < frames; ++f) {
                    size_t frameOffsetBytes = f * frameSizeBytes;

                    if (frameOffsetBytes + frameSizeBytes <= pixels.size()) {
                        std::memcpy(frameBuffer.data(), pixels.data() + frameOffsetBytes, frameSizeBytes);

                        glTextureSubImage3D(m_texture, 0, 0, 0, layer, tile_size.x, tile_size.y, 1,
                                            GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.data());
                        layer++;
                    }
                }
            }

            spng_ctx_free(ctx);
        }

        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenerateTextureMipmap(m_texture);
    }

}

