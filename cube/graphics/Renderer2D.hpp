#ifndef CUBE_RENDERER2D_HPP
#define CUBE_RENDERER2D_HPP

#include <vector>

#include "Font.hpp"
#include "RendererState.hpp"

namespace cube {

    class Renderer2D {
    public:
        Renderer2D();
        ~Renderer2D();

        void resize(const glm::vec2&);

        void begin();
        void end();

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

        void print(const std::string&, const glm::vec2& pos);

        void fill(uint32_t);
        void fill(const Color&);
        void fill(const Texture&, const glm::vec2& uv_min = {0,0}, const glm::vec2& uv_max = {1,1});
        void stroke(const Color&, float w = 1.f, bool loop = true);
        void text(const Font&, const Color&);
        void setJoin(JoinType);
        void setCap(CapType);

    private:
        void setState(State);
        void push(const glm::vec2& vertex, const glm::vec2& uv = {-1,-1});
        void toStroke(const std::vector<glm::vec2>&);
        void toFill(const std::vector<glm::vec2>&);
        void calcBox(const std::vector<glm::vec2>&);

        RendererState2D m_state;
    };

}

#endif //CUBE_RENDERER2D_HPP