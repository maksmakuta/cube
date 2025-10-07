#ifndef CUBE_RENDERER2D_HPP
#define CUBE_RENDERER2D_HPP

#include <vector>
#include <glm/mat4x4.hpp>

#include "Color.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace cube {

    enum class JoinType {
        Miter,
        Round,
        Bevel
    };

    enum class CapType {
        Flat,
        Square,
        Round
    };

    struct Vertex2D {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::uint col;
    };

    class Renderer2D {
    public:
        explicit Renderer2D(int alloc = 1024*1024);
        ~Renderer2D();

        void resize(const glm::vec2&);

        void begin();
        void end() const;
        void flush() const;

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

        void path(const std::vector<glm::vec2>&);

        void fill(const Color&);
        void fill(const Texture&);
        void stroke(const Color&, float w = 1.f, bool loop = true);
        void setJoin(JoinType);
        void setCap(CapType);

    private:
        void push(const glm::vec2& vertex, const glm::vec2& uv = {-1,-1});
        void toStroke(const std::vector<glm::vec2>&);
        void toFill(const std::vector<glm::vec2>&);
        void calcBox(const std::vector<glm::vec2>&);


        Color m_color;
        Shader m_shader;
        uint32_t m_vao{0};
        uint32_t m_vbo{0};
        bool is_fill{true};
        bool is_loop{true};
        float m_line_width{1.f};
        CapType m_cap = CapType::Flat;
        JoinType m_join = JoinType::Miter;
        const Texture *m_texture{nullptr};
        glm::vec4 m_box{-1};
        std::vector<Vertex2D> m_vertices;
        glm::mat4 m_projection{1.f};
    };

}

#endif //CUBE_RENDERER2D_HPP