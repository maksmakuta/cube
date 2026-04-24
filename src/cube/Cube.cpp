#include <random>
#include <ranges>
#include <cube/Cube.hpp>
#include <cube/utils/Logger.hpp>

#include <FastNoise/FastNoise.h>
#include <glad/glad.h>

namespace cube {

    constexpr auto DIST = 4;

    int randomSeed() {
        std::random_device dev;
        std::default_random_engine r(dev());
        return static_cast<int>(r());
    }

    Cube::Cube() {
        m_seed = randomSeed();
        Log::info("Seed : {}", m_seed);
        for (int cx = -DIST; cx <= DIST; cx++) {
            for (int cz = -DIST; cz <= DIST; cz++) {
                for (int cy = 0; cy <= 16; cy++) {
                    Log::info("Current chunk: [{},{},{}]", cx, cy, cz);
                    generateChunk({cx,cy,cz});
                    meshChunk({cx,cy,cz});
                }
            }
        }
    }

    Cube::~Cube() = default;

    void Cube::onDraw() {
        m_renderer.render(m_projection, m_camera.getViewMatrix(),m_camera.getPosition());
    }

    void Cube::onUpdate(const float dt) {
        auto newPos = m_camera.getPosition();
        const auto buttons = SDL_GetKeyboardState(nullptr);
        if (buttons[SDL_SCANCODE_W]) newPos += m_camera.getFront() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_S]) newPos -= m_camera.getFront() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_A]) newPos -= m_camera.getRight() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_D]) newPos += m_camera.getRight() * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_SPACE]) newPos += WORLD_UP * dt * CAMERA_SPEED;
        if (buttons[SDL_SCANCODE_LSHIFT]) newPos -= WORLD_UP * dt * CAMERA_SPEED;
        m_camera.move(newPos);
    }

    void Cube::onEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            m_camera.rotate(glm::vec2{e.motion.xrel, -e.motion.yrel} * MOUSE_SENSITIVITY);
        }
        if (e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            const auto aspect = static_cast<float>(e.window.data1) / static_cast<float>(e.window.data2);
            m_projection = glm::perspective(glm::radians(45.f),aspect,0.1f,512.f);
        }
    }

    void Cube::generateChunk(const glm::ivec3& chunkPos) {
    // 1. Create a Fractal node (FBm = Fractional Brownian Motion)
    auto perlin = FastNoise::New<FastNoise::Perlin>();
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();

    fractal->SetSource(perlin);
    fractal->SetOctaveCount(4);
    fractal->SetGain(0.75f);
    fractal->SetLacunarity(1.45f);

    std::vector<float> heightmap(CHUNK_SIZE.x * CHUNK_SIZE.z);
    const auto fpos = glm::vec3(chunkPos);
    auto chunkView = m_chunk.view();

    // 2. Use a much lower frequency (0.01f to 0.005f is usually the sweet spot)
    fractal->GenUniformGrid2D(
        heightmap.data(),
        fpos.x * CHUNK_SIZE.x, fpos.z * CHUNK_SIZE.z,
        CHUNK_SIZE.x, CHUNK_SIZE.z,
        1.0f,1.0f, // Lower frequency = smoother, larger features
        m_seed
    );

    for (int z = 0; z < CHUNK_SIZE.z; ++z) {
        for (int x = 0; x < CHUNK_SIZE.x; ++x) {
            float noiseVal = heightmap[z * CHUNK_SIZE.x + x];

            int terrainHeight = static_cast<int>(128.0f + noiseVal * 40.0f);

            for (int y = 0; y < CHUNK_SIZE.y; ++y) {
                int worldY = (chunkPos.y * CHUNK_SIZE.y) + y;

                if (worldY > terrainHeight) {
                    chunkView[x, y, z] = Air;
                } else if (worldY == terrainHeight) {
                    // Simple logic for sand near "sea level" (e.g., 110)
                    if (terrainHeight < 112) {
                         chunkView[x, y, z] = Sand;
                    } else {
                         chunkView[x, y, z] = Grass;
                    }
                } else if (worldY > terrainHeight - 4) {
                    chunkView[x, y, z] = Dirt;
                } else {
                    chunkView[x, y, z] = Stone;
                }
            }
        }
    }
}

    RenderableMesh getMesh(const Chunk& chunk, const glm::ivec3& chunkPos) {
        RenderableMesh mesh;
        mesh.pos = chunkPos;
        auto view = chunk.view();

        // Helper for face directions
        const glm::vec3 normals[6] = {
            { 0,  0,  1}, { 0,  0, -1}, // Front, Back
            { 0,  1,  0}, { 0, -1,  0}, // Top, Bottom
            { 1,  0,  0}, {-1,  0,  0}  // Right, Left
        };

        // Vertex offsets for a unit cube face
        // Organized as: Front, Back, Top, Bottom, Right, Left
        const glm::vec3 faceVertices[6][4] = {
            {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}}, // Front
            {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}}, // Back
            {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}}, // Top
            {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}}, // Bottom
            {{1,0,1}, {1,0,0}, {1,1,0}, {1,1,1}}, // Right
            {{0,0,0}, {0,0,1}, {0,1,1}, {0,1,0}}  // Left
        };

        uint32_t vertexCount = 0;

        for (int x = 0; x < CHUNK_SIZE.x; ++x) {
            for (int y = 0; y < CHUNK_SIZE.y; ++y) {
                for (int z = 0; z < CHUNK_SIZE.z; ++z) {

                    Block currentBlock = view[x, y, z];
                    if (currentBlock == Air) continue;

                    BlockData data = getBlockData(currentBlock);

                    // Check all 6 neighbors
                    for (int f = 0; f < 6; ++f) {
                        glm::ivec3 neighbor = glm::ivec3(x, y, z) + glm::ivec3(normals[f]);

                        bool isEdge = (neighbor.x < 0 || neighbor.x >= CHUNK_SIZE.x ||
                                       neighbor.y < 0 || neighbor.y >= CHUNK_SIZE.y ||
                                       neighbor.z < 0 || neighbor.z >= CHUNK_SIZE.z);

                        // Basic culling: only draw face if neighbor is Air or outside chunk
                        // Note: For perfect culling, you'd check neighboring Chunks here
                        if (isEdge || view[neighbor.x, neighbor.y, neighbor.z] == Air) {

                            // Select texture ID based on face
                            float texID = data.side;
                            if (normals[f].y > 0) texID = data.top;
                            if (normals[f].y < 0) texID = data.bottom;

                            // Add 4 vertices for the face
                            for (int v = 0; v < 4; ++v) {
                                constexpr glm::vec2 uv[4] = {
                                    glm::vec2(0,1), glm::vec2(1,1), glm::vec2(1,0), glm::vec2(0,0)
                                };

                                mesh.vertices.push_back({
                                    glm::vec3(x, y, z) + faceVertices[f][v],
                                    normals[f],
                                    glm::vec3(uv[v], texID) // Using Z for texture array index
                                });
                            }

                            // Add indices (Two triangles)
                            mesh.indices.push_back(vertexCount + 0);
                            mesh.indices.push_back(vertexCount + 1);
                            mesh.indices.push_back(vertexCount + 2);
                            mesh.indices.push_back(vertexCount + 0);
                            mesh.indices.push_back(vertexCount + 2);
                            mesh.indices.push_back(vertexCount + 3);

                            vertexCount += 4;
                        }
                    }
                }
            }
        }
        return mesh;
    }

    void Cube::meshChunk(const glm::ivec3& pos) {
        Log::info("Mesh chunk");
        const auto mesh = getMesh(m_chunk, pos);
        Log::info("Vertices: {}", mesh.vertices.size());
        Log::info("Indices: {}", mesh.indices.size());
        m_renderer.put(pos,mesh);
    }

}
