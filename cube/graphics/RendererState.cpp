#include "RendererState.hpp"

#include <glm/ext/matrix_clip_space.hpp>

#include "glad/gl.h"
#include "utils/AssetsPaths.hpp"

namespace cube {

    void RendererGLState2D::init(const int alloc) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(alloc * sizeof(Vertex2D)), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, pos)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, uv)));

        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, col)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader.fromName("render2d");
    }

    void RendererGLState2D::setProjection(const glm::mat4& projection) const {
        shader.use();
        shader.setMat4("proj", projection);
    }

    void RendererGLState2D::setTexture(const Texture& texture) const {
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        shader.setInt("image", 0);
    }

    void RendererGLState2D::flush(const std::vector<Vertex2D>& vertices, const int state) const {
        shader.use();
        shader.setInt("mode", state);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(vertices.size() * sizeof(Vertex2D)), vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void RendererGLState2D::release() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        shader.release();
        vao = 0;
        vbo = 0;
    }

    void RendererState2D::init() {
        gl_state.init();
    }

    void RendererState2D::flush() {
        if (vertices.empty()) return;
        if (current_state == State::Image) {
            gl_state.setTexture(image_state.texture);
        } else if (current_state == State::Text) {
            gl_state.setTexture(text_state.font.getTexture());
        }
        gl_state.flush(vertices,static_cast<int>(current_state));
        vertices.clear();
    }

    void RendererState2D::release() {
        gl_state.release();
    }

    void RendererState2D::resize(const int w, const int h) {
        const auto wf = static_cast<float>(w);
        const auto hf = static_cast<float>(h);
        projection = glm::ortho<float>(0.f, wf, hf, 0.f);
        glViewport(0, 0, w, h);

        gl_state.setProjection(projection);
    }

    void clear(const Color& c) {
        glClearColor(c.rf(), c.gf(), c.bf(), c.af());
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void clear(const uint32_t c) {
        clear(Color(c));
    }

}
