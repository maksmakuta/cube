#ifndef CUBE_RENDERER2D_HPP
#define CUBE_RENDERER2D_HPP

#include <memory>
#include <vector>

#include <glm/vec4.hpp>

#include "Color.hpp"
#include "Font.hpp"

namespace cube {

    enum class JoinType : uint8_t{
        Miter,
        Round,
        Bevel
    };

    enum class CapType : uint8_t{
        Flat,
        Square,
        Round
    };

    struct Vertex2D;
    struct RendererState2D;

    class Renderer2D {
    public:
        Renderer2D();
        ~Renderer2D();

        void resize(const glm::vec2&) const;

        void begin() const;
        void end() const;

        void point(const glm::vec2&) const;
        void line(const glm::vec2& a, const glm::vec2& b) const;
        void triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) const;
        void quad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d) const;
        void rect(const glm::vec2& origin, const glm::vec2& size) const;
        void rect(const glm::vec2& origin, const glm::vec2& size, float r) const;
        void rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec2& r) const;
        void rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec4& r) const;
        void circle(const glm::vec2& center, float r) const;
        void ellipse(const glm::vec2& center, const glm::vec2& r) const;
        void arc(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range) const;
        void pie(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range) const;
        void path(const std::vector<glm::vec2>&) const;

        void print(const std::string&, const glm::vec2& pos) const;

        void fill(uint32_t) const;
        void fill(const Color&) const;
        void fill(const Texture&, const glm::vec2& uv_min = {0,0}, const glm::vec2& uv_max = {1,1}) const;
        void stroke(const Color&, float w = 1.f, bool loop = true) const;
        void text(const Font&, const Color&) const;
        void setJoin(JoinType) const;
        void setCap(CapType) const;

    private:
        void push(const glm::vec2& vertex, const glm::vec2& uv = {-1,-1}) const;
        void toStroke(const std::vector<glm::vec2>&) const;
        void toFill(const std::vector<glm::vec2>&) const;
        void calcBox(const std::vector<glm::vec2>&) const;

        std::unique_ptr<RendererState2D> m_state;
    };

}

#endif //CUBE_RENDERER2D_HPP