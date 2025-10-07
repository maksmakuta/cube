#ifndef CUBE_RENDERER2D_HPP
#define CUBE_RENDERER2D_HPP

#include <vector>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

#include "Color.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace cube {

    struct Vertex2D {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::uint col;
    };

    class Renderer2D {
    public:
        Renderer2D();
        ~Renderer2D();

        void resize(const glm::vec2&);

        void begin();
        void end();
        void flush();

        void point(const glm::vec2&);
        void line(const glm::vec2& a, const glm::vec2& b);
        void triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
        void quad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d);

        void rect(const glm::vec2& origin, const glm::vec2& size);
        void rect(const glm::vec2& origin, const glm::vec2& size, float r);
        void rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec2& r);
        void rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec4& r);

        void circle(const glm::vec2& center, float r);
        void ellipse(const glm::vec2& center, const glm::vec2& r);
        void arc(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range);
        void pie(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range);

        void fill(const Color&);
        void fill(const Texture&);
        void stroke(const Color&, float w = 1.f);

    private:
        size_t m_len{0};
        uint32_t m_vao{0};
        uint32_t m_vbo{0};
        Color m_color{1.f, 1.f, 1.f, 1.f};
        Shader m_shader;
        const Texture *m_texture{nullptr};
        std::vector<Vertex2D> m_vertices;
        glm::mat4 m_projection{1.f};
        float m_lineWidth{1.f};
    };

}

#endif //CUBE_RENDERER2D_HPP