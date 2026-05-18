#ifndef CUBE_RENDERER_HPP
#define CUBE_RENDERER_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <cstdint>
#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include "cube/graphics/Camera.hpp"
#include "cube/graphics/Shader.hpp"
#include "cube/data/Mesh.hpp"

namespace cube {

    struct GLObject {
        uint32_t solid_vao;
        uint32_t solid_vbo;
        uint32_t solid_ebo;
        uint32_t solid_count;

        uint32_t blend_vao;
        uint32_t blend_vbo;
        uint32_t blend_ebo;
        uint32_t blend_count;
    };

    class Renderer final {
    public:
        Renderer();
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void uploadChunk(const glm::ivec3& pos, const Mesh& mesh);
        void unloadChunks(const glm::ivec3& pos, int render_distance);

        void render(const Camera& camera);

    private:
        void loadShaders();

        Shader m_shader;
        std::vector<glm::ivec3> m_list;
        std::unordered_map<glm::ivec3, GLObject> m_objects;
    };

}

#endif //CUBE_RENDERER_HPP
