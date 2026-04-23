#include <ranges>
#include <cube/Cube.hpp>
#include <cube/utils/Logger.hpp>

#include <FastNoise/FastNoise.h>
#include <glad/glad.h>

namespace cube {

    Cube::Cube() {
        generateChunk();
        meshChunk();
    }

    Cube::~Cube() = default;

    void Cube::onDraw() {
        // const auto pos = m_camera.getPosition();
        // const auto dir = m_camera.getDirection();
        // Log::info("Camera in [{:3.2f},{:3.2f},{:3.2f}] looking at [{:3.2f},{:3.2f}]",pos.x, pos.y, pos.z, dir.x, dir.y);
        m_renderer.render(m_projection, m_camera.getViewMatrix());
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
            m_projection = glm::perspective(glm::radians(45.f),aspect,0.1f,128.f);
        }
    }

    void Cube::generateChunk() {
        Log::info("Generating chunk");

        const auto node = FastNoise::New<FastNoise::Simplex>();
        auto fractal = FastNoise::New<FastNoise::FractalFBm>();
        fractal->SetSource(node);
        fractal->SetOctaveCount(3);

        float noiseOutput[CHUNK_LENGTH];
        float frequency = 0.85f;

        fractal->GenUniformGrid3D(
            noiseOutput,
            0.0f, 0.0f, 0.0f,
            16, 16, 16,
            frequency,
            frequency,
            frequency,
            1337
        );

        for (int z = 0; z < 16; ++z) {
            for (int y = 0; y < 16; ++y) {
                for (int x = 0; x < 16; ++x) {
                    float val = noiseOutput[x + (y * 16) + (z * 256)];

                    float density = val - (static_cast<float>(y) / 16.0f - 0.5f);

                    if (density > 0.0f) {
                        if (y > 12)      m_chunk.view()[x, y, z] = Grass;
                        else if (y > 8)  m_chunk.view()[x, y, z] = Dirt;
                        else             m_chunk.view()[x, y, z] = Stone;
                    }
                }
            }
        }
    }

    struct Face final {
        glm::vec3 verts[4];
        glm::vec3 normal;
    };

    const Face FACES[6] = {
        {{ {1,1,1}, {0,1,1}, {0,1,0}, {1,1,0} }, {0,1,0}},  // Top
        {{ {1,0,0}, {0,0,0}, {0,0,1}, {1,0,1} }, {0,-1,0}}, // Bottom
        {{ {1,1,1}, {1,1,0}, {1,0,0}, {1,0,1} }, {1,0,0}},  // Right
        {{ {0,1,0}, {0,1,1}, {0,0,1}, {0,0,0} }, {-1,0,0}}, // Left
        {{ {0,1,1}, {1,1,1}, {1,0,1}, {0,0,1} }, {0,0,1}},  // Front
        {{ {1,1,0}, {0,1,0}, {0,0,0}, {1,0,0} }, {0,0,-1}}  // Back
    };

    RenderableMesh getMesh(const Chunk& chunk, const glm::ivec3 chunkPos) {
        RenderableMesh mesh;
        mesh.pos = chunkPos;

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {
                    auto blockType = static_cast<Block>(chunk.view()[x,y,z]);
                    if (blockType == Air) continue;

                    const BlockData data = getBlockData(blockType);
                    glm::vec3 blockOffset(x, y, z);

                    for (int f = 0; f < 6; ++f) {
                        auto neighborDir = glm::ivec3(FACES[f].normal);
                        const glm::ivec3 neighborPos = glm::ivec3(x, y, z) + neighborDir;

                        bool isExposed = false;
                        if (neighborPos.x < 0 || neighborPos.x >= 16 ||
                            neighborPos.y < 0 || neighborPos.y >= 16 ||
                            neighborPos.z < 0 || neighborPos.z >= 16)
                            isExposed = true;
                        else if (chunk.view()[neighborPos.x,neighborPos.y,neighborPos.z] == Air)
                            isExposed = true;

                        if (isExposed) {
                            float texIndex;
                            if (f == 0)      texIndex = static_cast<float>(data.top);
                            else if (f == 1) texIndex = static_cast<float>(data.bottom);
                            else             texIndex = static_cast<float>(data.side);

                            auto startIdx = static_cast<uint32_t>(mesh.vertices.size());

                            for (int v = 0; v < 4; ++v) {
                                constexpr glm::vec2 uvs[4] = {{1,1}, {0,1}, {0,0}, {1,0}};
                                mesh.vertices.push_back({
                                    blockOffset + FACES[f].verts[v],
                                    FACES[f].normal,
                                    glm::vec3(uvs[v], texIndex)
                                });
                            }

                            mesh.indices.insert(mesh.indices.end(), {
                                startIdx, startIdx + 1, startIdx + 2,
                                startIdx, startIdx + 2, startIdx + 3
                            });
                        }
                    }
                }
            }
        }
        return mesh;
    }

    void Cube::meshChunk() {
        Log::info("Mesh chunk");
        constexpr auto pos = glm::ivec3(0);
        const auto mesh = getMesh(m_chunk, pos);
        Log::info("Vertices: {}", mesh.vertices.size());
        Log::info("Indices: {}", mesh.indices.size());
        m_renderer.put(pos,mesh);
    }

}
