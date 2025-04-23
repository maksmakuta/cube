#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Font.hpp"
#include "Shader.hpp"
#include "cube/utils/ColorUtils.hpp"

namespace cube {

    struct Vertex {
        glm::vec3 pos;
        glm::vec2 tex;
    };

    class Renderer {
    public:
        Renderer();

        void clear(const Color& hex = 0xFF101010);
        void resize(int w,int h);

        void draw();

        void applyFont(const Font&, const Color&);
        void text(const std::string& text, const glm::vec2& pos);
    private:
        Shader m_shader;
        glm::uint m_vao, m_vbo, m_ebo;
    };

}

#endif //RENDERER_HPP
