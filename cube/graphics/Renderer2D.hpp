#ifndef CUBE_RENDERER2D_HPP
#define CUBE_RENDERER2D_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Color.hpp"

namespace cube {

    struct Vertex2D {
        glm::vec2 pos;
        uint32_t  col;
        glm::vec2 tex;
    };

    class Renderer2D {
    public:
        Renderer2D();
        ~Renderer2D();

        void init();
        void deinit();
        void resize(const glm::ivec2& size);

        void line(const glm::vec2& p1, const glm::vec2& p2);
        void triangle(const glm::vec2& p1, const glm::vec2& p2);
        void rect(const glm::vec2& pos, const glm::vec2& size);
        void rect(const glm::vec2& pos, const glm::vec2& size, float r);
        void rect(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& r_top_bot);
        void rect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& r);
        void circle(const glm::vec2& p1, const glm::vec2& p2);
        void ellipse(const glm::vec2& p1, const glm::vec2& p2);

        void fill(uint32_t color);
        void fill(const Color&);

        void stroke(uint32_t color, float t = 1.f);
        void stroke(const Color&, float t = 1.f);

    private:
        uint m_vao{0};
        uint m_vbo{0};
        Color m_color;
        bool is_stroked{false};
        std::vector<Vertex2D> m_vertices;
    };

}

#endif //CUBE_RENDERER2D_HPP