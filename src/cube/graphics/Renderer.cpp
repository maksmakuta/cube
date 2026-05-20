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

        if (mesh.solid.vertices.empty() && mesh.transparent.vertices.empty()) {
            return;
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
        const auto vert = readFile(ASSETS + "shaders/voxel.vert");
        const auto frag = readFile(ASSETS + "shaders/voxel.frag");
        m_shader.compile(vert, frag);
    }

    const auto textures = std::vector<std::string>{
        "textures/brown_mushroom.png",
        "textures/melon_side.png",
        "textures/melon_top.png",
        "textures/pumpkin_side.png",
        "textures/pumpkin_top.png",
        "textures/red_mushroom.png",
        "textures/cactus/cactus_bottom.png",
        "textures/cactus/cactus_flower.png",
        "textures/cactus/cactus_side.png",
        "textures/cactus/cactus_top.png",
        "textures/coral/block/brain_coral_block.png",
        "textures/coral/block/bubble_coral_block.png",
        "textures/coral/block/fire_coral_block.png",
        "textures/coral/block/horn_coral_block.png",
        "textures/coral/block/tube_coral_block.png",
        "textures/coral/block/dead_brain_coral_block.png",
        "textures/coral/block/dead_bubble_coral_block.png",
        "textures/coral/block/dead_fire_coral_block.png",
        "textures/coral/block/dead_horn_coral_block.png",
        "textures/coral/block/dead_tube_coral_block.png",
        "textures/coral/brain_coral.png",
        "textures/coral/bubble_coral.png",
        "textures/coral/fire_coral.png",
        "textures/coral/horn_coral.png",
        "textures/coral/tube_coral.png",
        "textures/coral/dead_brain_coral.png",
        "textures/coral/dead_bubble_coral.png",
        "textures/coral/dead_fire_coral.png",
        "textures/coral/dead_horn_coral.png",
        "textures/coral/dead_tube_coral.png",
        "textures/decorations/bush.png",
        "textures/decorations/cave_vines_lit.png",
        "textures/decorations/cave_vines_plant_lit.png",
        "textures/decorations/cave_vines_plant.png",
        "textures/decorations/cave_vines.png",
        "textures/decorations/dead_bush.png",
        "textures/decorations/fern.png",
        "textures/decorations/firefly_bush.png",
        "textures/decorations/grass.png",
        "textures/decorations/short_dry_grass.png",
        "textures/decorations/short_grass.png",
        "textures/decorations/sugar_cane.png",
        "textures/decorations/sweet_berry_bush.png",
        "textures/decorations/tall_dry_grass.png",
        "textures/flowers/allium.png",
        "textures/flowers/azure_bluet.png",
        "textures/flowers/blue_orchid.png",
        "textures/flowers/cornflower.png",
        "textures/flowers/dandelion.png",
        "textures/flowers/orange_tulip.png",
        "textures/flowers/oxeye_daisy.png",
        "textures/flowers/pink_tulip.png",
        "textures/flowers/poppy.png",
        "textures/flowers/red_tulip.png",
        "textures/flowers/white_tulip.png",
        "textures/leaves/acacia_leaves.png",
        "textures/leaves/birch_leaves.png",
        "textures/leaves/cherry_leaves.png",
        "textures/leaves/dark_oak_leaves.png",
        "textures/leaves/jungle_leaves.png",
        "textures/leaves/oak_leaves.png",
        "textures/leaves/pale_oak_leaves.png",
        "textures/leaves/spruce_leaves.png",
        "textures/logs/acacia_log.png",
        "textures/logs/acacia_log_top.png",
        "textures/logs/birch_log.png",
        "textures/logs/birch_log_top.png",
        "textures/logs/cherry_log.png",
        "textures/logs/cherry_log_top.png",
        "textures/logs/dark_oak_log.png",
        "textures/logs/dark_oak_log_top.png",
        "textures/logs/jungle_log.png",
        "textures/logs/jungle_log_top.png",
        "textures/logs/oak_log.png",
        "textures/logs/oak_log_top.png",
        "textures/logs/pale_oak_log.png",
        "textures/logs/pale_oak_log_top.png",
        "textures/logs/spruce_log.png",
        "textures/logs/spruce_log_top.png",
        "textures/natural/amethyst_block.png",
        "textures/natural/andesite.png",
        "textures/natural/basalt_side.png",
        "textures/natural/basalt_top.png",
        "textures/natural/bedrock.png",
        "textures/natural/blackstone.png",
        "textures/natural/blackstone_top.png",
        "textures/natural/blue_ice.png",
        "textures/natural/calcite.png",
        "textures/natural/clay.png",
        "textures/natural/coarse_dirt.png",
        "textures/natural/cobblestone.png",
        "textures/natural/deepslate.png",
        "textures/natural/deepslate_top.png",
        "textures/natural/diorite.png",
        "textures/natural/dirt.png",
        "textures/natural/dripstone_block.png",
        "textures/natural/granite.png",
        "textures/natural/grass_block_side_overlay.png",
        "textures/natural/grass_block_side.png",
        "textures/natural/grass_block_snow.png",
        "textures/natural/grass_block_top.png",
        "textures/natural/gravel.png",
        "textures/natural/ice.png",
        "textures/natural/mud.png",
        "textures/natural/packed_ice.png",
        "textures/natural/red_sand.png",
        "textures/natural/red_sandstone.png",
        "textures/natural/sand.png",
        "textures/natural/sandstone.png",
        "textures/natural/stone.png",
        "textures/natural/tuff.png",
        "textures/sapling/acacia_sapling.png",
        "textures/sapling/birch_sapling.png",
        "textures/sapling/cherry_sapling.png",
        "textures/sapling/dark_oak_sapling.png",
        "textures/sapling/jungle_sapling.png",
        "textures/sapling/oak_sapling.png",
        "textures/sapling/pale_oak_sapling.png",
        "textures/sapling/spruce_sapling.png",
        "textures/tall/large_fern_bottom.png",
        "textures/tall/large_fern_top.png",
        "textures/tall/peony_bottom.png",
        "textures/tall/peony_top.png",
        "textures/tall/rose_bush_bottom.png",
        "textures/tall/rose_bush_top.png",
        "textures/tall/tall_grass_bottom.png",
        "textures/tall/tall_grass_top.png",
        "textures/tall/tall_seagrass_bottom.png",
        "textures/tall/tall_seagrass_top.png",
        "textures/kelp_plant.png",
        "textures/lava_still.png",
        "textures/seagrass.png",
        "textures/water_still.png",
    };

    struct TextureMetadata {
        std::string path;
        std::vector<char> fileBuffer;
        int frameCount = 1;
    };

    void Renderer::loadTextures(const glm::ivec2& tile_size) {
        std::vector<TextureMetadata> metaList;
        int totalLayers = 0;

        for (const auto& path : textures) {
            const auto filepath = ASSETS + path;
            std::ifstream file(filepath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cerr << "Failed to open asset path: " << filepath << "\n";
                continue;
            }

            size_t fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            TextureMetadata meta;
            meta.path = path;
            meta.fileBuffer.resize(fileSize);
            file.read(meta.fileBuffer.data(), fileSize);

            spng_ctx* ctx = spng_ctx_new(0);
            if (!ctx) continue;

            if (spng_set_png_buffer(ctx, meta.fileBuffer.data(), fileSize) == 0) {
                spng_ihdr ihdr{};
                if (spng_get_ihdr(ctx, &ihdr) == 0) {
                    meta.frameCount = static_cast<int>(ihdr.height) / tile_size.y;

                    if (meta.frameCount < 1) meta.frameCount = 1;

                    totalLayers += meta.frameCount;
                    metaList.push_back(std::move(meta));
                }
            }
            spng_ctx_free(ctx);
        }

        if (totalLayers == 0) {
            std::cerr << "No valid textures found to allocate.\n";
            return;
        }

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);
        const int mipLevels = 1 + static_cast<int>(std::floor(std::log2(tile_size.x)));
        glTextureStorage3D(m_texture, mipLevels, GL_RGBA8, tile_size.x, tile_size.y, totalLayers);

        int currentLayer = 0;

        for (const auto&[path, fileBuffer, frameCount] : metaList) {
            spng_ctx* ctx = spng_ctx_new(0);
            if (!ctx) continue;

            if (spng_set_png_buffer(ctx, fileBuffer.data(), fileBuffer.size())) {
                spng_ctx_free(ctx);
                continue;
            }

            size_t outSize;
            if (spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &outSize)) {
                spng_ctx_free(ctx);
                continue;
            }

            std::vector<uint8_t> pixels(outSize);
            if (spng_decode_image(ctx, pixels.data(), outSize, SPNG_FMT_RGBA8, 0)) {
                std::cerr << "Failed to decode PNG: " << path << "\n";
                spng_ctx_free(ctx);
                continue;
            }

            //std::cout << "Uploading: " << path
            //          << " | Starting Layer: " << currentLayer
            //          << " | Frames: " << frameCount << "\n";

            const size_t frameSizeBytes = tile_size.x * tile_size.y * 4;

            for (int f = 0; f < frameCount; ++f) {
                size_t frameOffsetBytes = f * frameSizeBytes;

                if (frameOffsetBytes + frameSizeBytes <= pixels.size()) {
                    glTextureSubImage3D(m_texture, 0, 0, 0, currentLayer, tile_size.x, tile_size.y, 1,
                                        GL_RGBA, GL_UNSIGNED_BYTE, pixels.data() + frameOffsetBytes);
                    currentLayer++;
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

