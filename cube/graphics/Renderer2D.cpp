#include "Renderer2D.hpp"

#include "utils/AssetsPaths.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace cube {
    Renderer2D::Renderer2D() : m_shader(
        Shader::fromFiles(
            getShader("render2d_vert.glsl"),
            getShader("render2d_frag.glsl")
        )
    ) {

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 1024 * 1024 * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

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

    void Renderer2D::end() {
        flush();
    }

    void Renderer2D::flush() {
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
        const auto dir = glm::normalize(b - a);
        const auto norm = glm::vec2(-dir.y, dir.x) * m_lineWidth;
        quad(
            a + norm,
            a - norm,
            b + norm,
            b - norm
        );
    }

    void Renderer2D::triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c){
        m_vertices.emplace_back(a, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        m_vertices.emplace_back(b, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        m_vertices.emplace_back(c, glm::vec2(0.f), static_cast<uint32_t>(m_color));
    }

    void Renderer2D::quad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d){
        m_vertices.emplace_back(a, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        m_vertices.emplace_back(b, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        m_vertices.emplace_back(c, glm::vec2(0.f), static_cast<uint32_t>(m_color));

        m_vertices.emplace_back(a, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        m_vertices.emplace_back(c, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        m_vertices.emplace_back(d, glm::vec2(0.f), static_cast<uint32_t>(m_color));
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size){
        quad(
            origin,
            origin + glm::vec2(size.x, 0.f),
            origin + size,
            origin + glm::vec2(0.f, size.y)
        );
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, float r){
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

        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i    ) * step;
            const float t1 = static_cast<float>(i + 1) * step;

            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            glm::vec2 p1 = center + glm::vec2(std::cos(t1), std::sin(t1)) * r;

            // Triangle fan (center, edge0, edge1)
            m_vertices.emplace_back(center, glm::vec2(0.f), static_cast<uint32_t>(m_color));
            m_vertices.emplace_back(p0, glm::vec2(0.f), static_cast<uint32_t>(m_color));
            m_vertices.emplace_back(p1, glm::vec2(0.f), static_cast<uint32_t>(m_color));
        }
    }

    void Renderer2D::arc(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){

    }

    void Renderer2D::pie(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){

    }

    void Renderer2D::fill(const Color& c){
        m_color = c;
    }

    void Renderer2D::fill(const Texture& t){
        m_texture = &t;
    }

    void Renderer2D::stroke(const Color& c, const float w){
        fill(c);
        m_lineWidth = w;
    }

}
