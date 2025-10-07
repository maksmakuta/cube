#include "Renderer2D.hpp"

#include <functional>
#include <optional>
#include <glm/gtc/matrix_transform.hpp>

#include "utils/AssetsPaths.hpp"

namespace cube {

    constexpr auto maxMiterLength = 50.f;

    Renderer2D::Renderer2D(const int alloc) : m_shader(
        Shader::fromFiles(
            getShader("render2d_vert.glsl"),
            getShader("render2d_frag.glsl")
        )
    ) {

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(alloc * sizeof(Vertex2D)), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, pos)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, uv)));

        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, col)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    Renderer2D::~Renderer2D() = default;

    void Renderer2D::resize(const glm::vec2& view) {
        m_projection = glm::ortho(0.f,view.x , view.y, 0.f);
        glViewport(0, 0, static_cast<int>(view.x), static_cast<int>(view.y));
    }

    void Renderer2D::begin() {
        m_vertices.clear();
    }

    void Renderer2D::end() const {
        flush();
    }

    void Renderer2D::flush() const {
        if (m_vertices.empty()) return;

        m_shader.use();
        m_shader.setMat4("u_Projection", m_projection);

        if (m_texture != nullptr) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture->id());
            m_shader.setInt("u_Texture", 0);
        }

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(m_vertices.size() * sizeof(Vertex2D)), m_vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer2D::point(const glm::vec2& center){
        rect(center - glm::vec2(0.5f), glm::vec2(1.f));
    }

    void Renderer2D::line(const glm::vec2& a, const glm::vec2& b){
        toStroke({a,b});
    }

    void Renderer2D::triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c){
        const auto path = {a,b,c};
        if (is_fill)
            toFill(path);
        else
            toStroke(path);
    }

    void Renderer2D::quad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d){
        const auto path = {a,b,c,d};
        if (is_fill)
            toFill(path);
        else
            toStroke(path);
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size){
        quad(
            origin,
            origin + glm::vec2(size.x, 0.f),
            origin + size,
            origin + glm::vec2(0.f, size.y)
        );
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const float r){
        rect(origin, size, glm::vec4(r));
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec2& r){
        rect(origin, size, glm::vec4(r.x, r.y, r.x, r.y));
    }


    void subArc(std::vector<glm::vec2>& path, const glm::vec2& center, const glm::vec2& r, const glm::vec2& range) {
        constexpr int segments = 8; // Segments per corner arc
        const float step = (range.y - range.x) / segments;

        for (int i = 0; i <= segments; ++i) {
            const float angle = range.x + static_cast<float>(i) * step;
            path.emplace_back(center + glm::vec2(std::cos(angle), std::sin(angle)) * r);
        }
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec4& r){
        const glm::vec4 radii = glm::min(r, glm::vec4(glm::min(size.x, size.y) * 0.5f));

        std::vector<glm::vec2> path;
        path.reserve(64);

        if (radii.x > 0.f) {
            subArc(path, origin + glm::vec2(radii.x, radii.x),
                   glm::vec2(radii.x), glm::vec2(glm::pi<float>(), glm::half_pi<float>() * 3.f));
        } else {
            path.emplace_back(origin);
        }

        if (radii.y > 0.f) {
            subArc(path, origin + glm::vec2(size.x - radii.y, radii.y),
                   glm::vec2(radii.y), glm::vec2(glm::half_pi<float>() * 3.f, glm::two_pi<float>()));
        } else {
            path.emplace_back(origin + glm::vec2(size.x, 0.f));
        }

        if (radii.z > 0.f) {
            subArc(path, origin + glm::vec2(size.x - radii.z, size.y - radii.z),
                   glm::vec2(radii.z), glm::vec2(0.f, glm::half_pi<float>()));
        } else {
            path.emplace_back(origin + size);
        }

        if (radii.w > 0.f) {
            subArc(path, origin + glm::vec2(radii.w, size.y - radii.w),
                   glm::vec2(radii.w), glm::vec2(glm::half_pi<float>(), glm::pi<float>()));
        } else {
            path.emplace_back(origin + glm::vec2(0.f, size.y));
        }

        if (is_fill)
            toFill(path);
        else
            toStroke(path);
    }

    void Renderer2D::circle(const glm::vec2& center, const float r){
        ellipse(center, glm::vec2(r));
    }

    void Renderer2D::ellipse(const glm::vec2& center, const glm::vec2& r){
        constexpr int segments = 32;
        constexpr float step = glm::two_pi<float>() / segments;
        auto path = std::vector<glm::vec2>{};
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i) * step;
            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            path.emplace_back(p0);
        }
        if (is_fill)
            toFill(path);
        else
            toStroke(path);
    }

    void Renderer2D::arc(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){
        constexpr int segments = 32;
        const float step = (range.y - range.x) / segments;
        auto path = std::vector<glm::vec2>{};
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i    ) * step + range.x;
            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            path.emplace_back(p0);
        }
        if (is_fill)
            toFill(path);
        else
            toStroke(path);
    }

    void Renderer2D::pie(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){
        constexpr int segments = 32;
        const float step = (range.y - range.x) / segments;
        auto path = std::vector<glm::vec2>{};
        path.emplace_back(center);
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i    ) * step + range.x;
            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            path.emplace_back(p0);
        }
        if (is_fill)
            toFill(path);
        else
            toStroke(path);
    }

    void Renderer2D::path(const std::vector<glm::vec2>& p) {
        if (is_fill)
            toFill(p);
        else
            toStroke(p);
    }

    void Renderer2D::fill(const Color& c){
        if (!is_fill || m_texture != nullptr) {
            flush();
        }
        m_color = c;
        is_fill = true;
        m_line_width = 1.f;
    }

    void Renderer2D::fill(const Texture& t){
        if (!is_fill || m_texture == nullptr) {
            flush();
        }
        m_texture = &t;
        is_fill = true;
        m_line_width = 1.f;
    }

    void Renderer2D::stroke(const Color& c, const float w, const bool loop){
        if (is_fill || m_texture == nullptr) {
            flush();
        }
        is_fill = false;
        m_color = c;
        m_line_width = w;
        is_loop = loop;
    }

    void Renderer2D::setJoin(const JoinType j){
        m_join = j;
    }

    void Renderer2D::setCap(const CapType c){
        m_cap = c;
    }

    void Renderer2D::push(const glm::vec2& vertex, const glm::vec2& uv) {
        m_vertices.emplace_back(vertex, uv, static_cast<uint32_t>(m_color));
    }

    void Renderer2D::toFill(const std::vector<glm::vec2>& path) {
        for (auto i = 1; i + 1 < path.size(); ++i) {
            push(path.front());
            push(path[i]);
            push(path[i+1]);
        }
    }

    struct Line {

        Line(const glm::vec2& start, const glm::vec2& end) : a(start), b(end){}

        Line operator + (const glm::vec2& v) const {
            return {a + v, b + v};
        }

        Line operator - (const glm::vec2& v) const {
            return {a - v, b - v};
        }

        [[nodiscard]] glm::vec2 dir(const bool n = true) const {
            const auto d = b - a;
            if (n) return glm::normalize(d);
            return d;
        }

        [[nodiscard]] glm::vec2 normal() const {
            const auto d = dir();
            return {-d.y, d.x};
        }

        glm::vec2 a,b;
    };

    std::optional<glm::vec2> intersect(const Line& l1, const Line& l2, const bool infinite = false) {
        const auto p = l1.a;
        const auto r = l1.b - l1.a;
        const auto q = l2.a;
        const auto s = l2.b - l2.a;

        const auto rxs = r.x * s.y - r.y * s.x;
        if (std::abs(rxs) < 1e-6f)
            return std::nullopt;

        const auto qp = q - p;
        const auto t = (qp.x * s.y - qp.y * s.x) / rxs;
        const auto u = (qp.x * r.y - qp.y * r.x) / rxs;

        if (!infinite && (t < 0.f || t > 1.f || u < 0.f || u > 1.f)){
            return std::nullopt;
        }

        return p + t * r;
    }

    struct Segment {

        Segment(const Line& middle, float thick) : top(middle + middle.normal() * thick),
            bottom(middle - middle.normal() * thick) , center(middle){}

        Line top;
        Line bottom;
        Line center;
    };

    void Renderer2D::toStroke(const std::vector<glm::vec2>& path) {
        if (path.size() < 2) return;

        const auto thick = m_line_width * 0.5f;

        auto segments = std::vector<Segment>();

        for (auto i = 0; i + 1 < path.size(); ++i) {
            segments.emplace_back(Line(path[i], path[i+1]), thick);
        }

        if (is_loop) {
            segments.emplace_back(Line(path.front(), path.back()), thick);
        }else {
            if (m_cap == CapType::Round) {
                //TODO(Round cap)
            }
            if (m_cap == CapType::Square) {
                segments.back().top.b += segments.back().center.dir() * thick;
                segments.back().bottom.b += segments.back().center.dir() * thick;
                segments.front().top.a -= segments.front().center.dir() * thick;
                segments.front().bottom.a -= segments.front().center.dir() * thick;
            }
        }

        for (auto i = 0; i + 1 < segments.size(); ++i) {
            auto& s1 = segments[i];
            auto& s2 = segments[i + 1];

            const glm::vec2 d1 = s1.center.dir();
            const glm::vec2 d2 = s2.center.dir();
            const bool left = (d1.x * d2.y - d1.y * d2.x) > 0.f;

            Line& outer1 = left ? s1.top : s1.bottom;
            Line& inner1 = left ? s1.bottom : s1.top;
            Line& outer2 = left ? s2.top : s2.bottom;
            Line& inner2 = left ? s2.bottom : s2.top;

            if (auto p = intersect(outer1, outer2, true)) {
                outer1.b = *p;
                outer2.a = *p;
            }
            if (auto p = intersect(inner1, inner2, false)) {
                inner1.b = *p;
                inner2.a = *p;
            }
        }

        for (const auto& s : segments) {
            push(s.top.a);
            push(s.top.b);
            push(s.bottom.a);
            push(s.top.b);
            push(s.bottom.a);
            push(s.bottom.b);

        }

    }

}
