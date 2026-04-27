#include <cube/data/World.hpp>

#include <mutex>

#include <cube/graphics/Renderer.hpp>
#include <cube/utils/Logger.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace cube {

    World::World(const int seed) : m_generator(seed), m_threadPool() {}

    std::shared_ptr<Chunk> World::getChunk(const glm::ivec3& chunkPos) {
        std::shared_lock lock(m_mapMutex);
        if (const auto it = m_chunks.find(chunkPos); it != m_chunks.end()) {
            return it->second;
        }
        return nullptr;
    }

    Block World::getBlock(const glm::ivec3& worldPos) {
        if (worldPos.y < 0 || worldPos.y >= 256) return Air;

        const glm::ivec3 cPos = {worldPos.x >> 4, worldPos.y >> 4, worldPos.z >> 4};
        std::shared_lock lock(m_mapMutex);

        if (const auto it = m_chunks.find(cPos); it != m_chunks.end() && it->second) {
            return it->second->view()[worldPos.x & 15, worldPos.y & 15, worldPos.z & 15];
        }
        return Air;
    }

    ChunkNeighbors World::getNeighbors(const glm::ivec3 &pos) {
        ChunkNeighbors neighbors{};
        std::shared_lock lock(m_mapMutex);

        if (const auto it = m_chunks.find(pos); it != m_chunks.end()) neighbors.center = it->second;
        if (const auto i = m_chunks.find(pos + glm::ivec3(1,0,0)); i != m_chunks.end()) neighbors.px = i->second;
        if (const auto i = m_chunks.find(pos + glm::ivec3(0,1,0)); i != m_chunks.end()) neighbors.py = i->second;
        if (const auto i = m_chunks.find(pos + glm::ivec3(0,0,1)); i != m_chunks.end()) neighbors.pz = i->second;
        if (const auto i = m_chunks.find(pos - glm::ivec3(1,0,0)); i != m_chunks.end()) neighbors.nx = i->second;
        if (const auto i = m_chunks.find(pos - glm::ivec3(0,1,0)); i != m_chunks.end()) neighbors.ny = i->second;
        if (const auto i = m_chunks.find(pos - glm::ivec3(0,0,1)); i != m_chunks.end()) neighbors.nz = i->second;

        return neighbors;
    }

    std::vector<glm::ivec3> World::loadArea(const glm::ivec3& centerChunk, const int radius) {
        std::vector<glm::ivec3> pending_coords;
        pending_coords.reserve(16 * 2 * radius);
        const int radiusSq = radius * radius;

        {
            std::unique_lock lock(m_mapMutex);

            for (int x = -radius; x <= radius; ++x) {
                for (int z = -radius; z <= radius; ++z) {
                    if (x * x + z * z > radiusSq) continue;

                    for (int y = 0; y < 16; ++y) {
                        const glm::ivec3 targetPos = {centerChunk.x + x, y, centerChunk.z + z};

                        if (!m_chunks.contains(targetPos) && !m_generating.contains(targetPos)) {
                            m_generating.insert(targetPos);
                            pending_coords.push_back(targetPos);
                        }
                    }
                }
            }
        }

        std::ranges::sort(pending_coords,
            [&centerChunk](const glm::ivec3& a, const glm::ivec3& b) {
                return glm::distance2(glm::vec3(a), glm::vec3(centerChunk)) <
                       glm::distance2(glm::vec3(b), glm::vec3(centerChunk));
            });

        for (const auto& targetPos : pending_coords) {
            m_threadPool.enqueue([this, targetPos] {
                const Chunk generatedChunk = m_generator.generate(targetPos);
                m_chunkQueue.push(ChunkResult{targetPos, generatedChunk});
            });
        }

        m_threadPool.notifyAll();
        return pending_coords;
    }

    inline int distXZ(const glm::ivec3& a, const glm::ivec3& b) {
        const int dx = a.x - b.x;
        const int dz = a.z - b.z;
        return dx * dx + dz * dz;
    }

    void World::unloadFarChunks(const glm::ivec3 &centerChunk, const int radius, const UnloadCallback& onUnload) {
        int removedCount = 0;
        std::unique_lock lock(m_mapMutex);

        for (auto it = m_chunks.begin(); it != m_chunks.end(); ) {
            if (distXZ(it->first, centerChunk) > radius) {
                if (onUnload) onUnload(it->first);
                it = m_chunks.erase(it);
                removedCount++;
            } else {
                ++it;
            }
        }

        if (removedCount > 0) {
            Log::info("Unloaded {} chunks", removedCount);
        }
    }

    void World::ensureChunkExists(const glm::ivec3& pos) {
        std::unique_lock lock(m_mapMutex);
        if (!m_chunks.contains(pos) && !m_generating.contains(pos)) {
            m_generating.insert(pos);
            m_threadPool.enqueue([this, pos] {
                const Chunk generatedChunk = m_generator.generate(pos);
                m_chunkQueue.push(ChunkResult{pos, generatedChunk});
            });
        }
    }

    void World::processAsyncResults(Renderer& renderer) {
        if (const auto newChunks = m_chunkQueue.pop_all(); !newChunks.empty()) {
            std::unique_lock lock(m_mapMutex);

            for (auto [pos, chunk] : newChunks) {
                const auto chunkPtr = std::make_shared<Chunk>(chunk);
                chunkPtr->setState(ChunkState::Generated);
                m_chunks[pos] = chunkPtr;
                m_generating.erase(pos);
            }

            auto tryQueueMesh = [&](const glm::ivec3& p) {
                const auto it = m_chunks.find(p);
                if (it == m_chunks.end()) return;

                const auto& chunk = it->second;
                if (chunk->state() != ChunkState::Generated) return;

                ChunkNeighbors n{};
                n.center = chunk;
                if (const auto i = m_chunks.find(p + glm::ivec3(1,0,0)); i != m_chunks.end()) n.px = i->second;
                if (const auto i = m_chunks.find(p - glm::ivec3(1,0,0)); i != m_chunks.end()) n.nx = i->second;
                if (const auto i = m_chunks.find(p + glm::ivec3(0,1,0)); i != m_chunks.end()) n.py = i->second;
                if (const auto i = m_chunks.find(p - glm::ivec3(0,1,0)); i != m_chunks.end()) n.ny = i->second;
                if (const auto i = m_chunks.find(p + glm::ivec3(0,0,1)); i != m_chunks.end()) n.pz = i->second;
                if (const auto i = m_chunks.find(p - glm::ivec3(0,0,1)); i != m_chunks.end()) n.nz = i->second;

                if (n.isValid()) {
                    chunk->setState(ChunkState::Meshing);

                    m_threadPool.enqueue([this, p, n] {
                        RenderableMesh mesh = getMesh(n, p,
                            m_generator.getTemperature(p),
                            m_generator.getHumidity(p));
                        m_meshQueue.push(std::move(mesh));
                    });
                }
            };

            for (const auto&[pos, chunk] : newChunks) {
                tryQueueMesh(pos);                         // Check the chunk itself
                tryQueueMesh(pos + glm::ivec3(1, 0, 0));   // Check +X neighbor
                tryQueueMesh(pos - glm::ivec3(1, 0, 0));   // Check -X neighbor
                tryQueueMesh(pos + glm::ivec3(0, 1, 0));   // Check +Y neighbor
                tryQueueMesh(pos - glm::ivec3(0, 1, 0));   // Check -Y neighbor
                tryQueueMesh(pos + glm::ivec3(0, 0, 1));   // Check +Z neighbor
                tryQueueMesh(pos - glm::ivec3(0, 0, 1));   // Check -Z neighbor
            }
        }

        auto newMeshes = m_meshQueue.pop_all();
        int uploaded = 0;
        for (auto& mesh : newMeshes) {
            if (constexpr int uploadLimit = 8; uploaded >= uploadLimit) {
                m_meshQueue.push(std::move(mesh));
                continue;
            }

            {
                std::shared_lock lock(m_mapMutex);
                if (auto it = m_chunks.find(mesh.pos); it != m_chunks.end()) {
                    it->second->setState(ChunkState::Renderable);
                    renderer.put(mesh);
                    uploaded++;
                }
            }
        }
    }

}