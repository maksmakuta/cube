#include "Renderer2D.hpp"

#include <functional>

#include "utils/AssetsPaths.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace cube {
    Renderer2D::Renderer2D(int alloc) : m_shader(
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex2D), (void*)offsetof(Vertex2D, col));

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

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec4& r){

    }

    void Renderer2D::circle(const glm::vec2& center, const float r){
        ellipse(center, glm::vec2(r));
    }

    void Renderer2D::ellipse(const glm::vec2& center, const glm::vec2& r){
        constexpr int segments = 32;
        constexpr float step = glm::two_pi<float>() / segments;
        auto path = std::vector<glm::vec2>{};
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i    ) * step;
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
            toStroke(path,false);
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

    void Renderer2D::fill(const Color& c){
        m_color = c;
        is_fill = true;
        m_line_width = 1.f;
    }

    void Renderer2D::fill(const Texture& t){
        m_texture = &t;
        is_fill = true;
        m_line_width = 1.f;
    }

    void Renderer2D::stroke(const Color& c, const float w){
        is_fill = false;
        m_color = c;
        m_line_width = w;
    }

    void Renderer2D::setJoin(JoinType j){
        m_join = j;
    }

    void Renderer2D::setCap(CapType c){
        m_cap = c;
    }

    void Renderer2D::push(const glm::vec2& vertex, const glm::vec2& uv) {
        m_vertices.emplace_back(vertex, uv, static_cast<uint32_t>(m_color));
    }

    struct Line {
        Line(const glm::vec2& start, const glm::vec2& end) : a(start), b(end) {}

        Line operator + (const glm::vec2& w) const {
            return {a + w, b + w};
        }

        Line operator - (const glm::vec2& w) const {
            return {a - w, b - w};
        }

        [[nodiscard]] glm::vec2 dir(bool n = true) const {
            const auto d = b - a;
            if (n) return glm::normalize(d);
            return d;
        }

        [[nodiscard]] glm::vec2 norm() const {
            const auto d = dir();
            return {-d.y, d.x};
        }

        glm::vec2 a;
        glm::vec2 b;
    };

    struct Segment {
        Segment(const Line& l, const float w) :
            top(l + l.norm() * w), bottom(l - l.norm() * w) {}

        void toVertices(const std::function<void(const glm::vec2&)>& callback) const {
            callback(top.a);
            callback(top.b);
            callback(bottom.a);

            callback(top.b);
            callback(bottom.a);
            callback(bottom.b);
        }

        Line top;
        Line bottom;
    };

    void Renderer2D::toStroke(const std::vector<glm::vec2>& path, bool loop) {
        if (path.size() < 2) return;

        const float half = m_line_width * 0.5f;
        const size_t count = path.size();

        std::vector<Segment> segments;
        for (size_t i = 0; i < count - 1; ++i) {
            segments.emplace_back(Line(path[i], path[i + 1]), half);
        }

        if (loop) {
            segments.emplace_back(Line(path.back(), path.front()), half);
        }

        for (const auto& s : segments) {
            s.toVertices([this](const glm::vec2& v){
                push(v);
            });
        }

    }

    void Renderer2D::toFill(const std::vector<glm::vec2>& path) {
        for (auto i = 1; i + 1 < path.size(); ++i) {
            push(path.front());
            push(path[i]);
            push(path[i+1]);
        }
    }


}
