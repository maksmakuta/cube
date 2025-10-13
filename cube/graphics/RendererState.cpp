#include "RendererState.hpp"

#include <iostream>
#include <ostream>
#include <glm/ext/matrix_clip_space.hpp>

#include "TextureBuilder.hpp"
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


    void RendererGLState3D::init(const int alloc) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(alloc * sizeof(Vertex3D)), nullptr, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void *>(offsetof(Vertex3D, nor)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void *>(offsetof(Vertex3D, tex)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void *>(offsetof(Vertex3D, tid)));
        glEnableVertexAttribArray(3);

        shader.fromName("render3d");
        textures = TextureBuilder()
            .setFilter(TextureFilter::Nearest)
            .buildArray(getTexture("blocks/"));
    }

    void RendererGLState3D::flush(const std::vector<Vertex3D>& vertices) const {
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.getId());
        shader.setInt("textures", 0);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(vertices.size() * sizeof(Vertex3D)), vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void RendererGLState3D::release() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        textures.release();
        shader.release();
        vao = 0;
        vbo = 0;
    }

    void RendererGLState3D::setProjection(const glm::mat4& mat) const {
        shader.use();
        shader.setMat4("proj", mat);
    }

    void RendererGLState3D::setView(const glm::mat4& mat) const {
        shader.use();
        shader.setMat4("view", mat);
    }

    void RendererGLState3D::setModel(const glm::mat4& mat) const {
        shader.use();
        shader.setMat4("model", mat);
    }

    void RendererState3D::init() {
        gl_state.init();
        std::cout << "textures: " << gl_state.textures.getLength() << std::endl;
    }

    void RendererState3D::flush() {
        if (vertices.empty()) return;
        gl_state.setProjection(projection);
        gl_state.setView(view);
        gl_state.setModel(model);

        gl_state.flush(vertices);
        vertices.clear();
    }

    void RendererState3D::release() {
        gl_state.release();
    }

    void RendererState3D::resize(const int w, const int h) {
        const auto wf = static_cast<float>(w);
        const auto hf = static_cast<float>(h);
        projection = glm::perspectiveFov(70.f,wf,hf,0.1f,100.f);
    }

    void clear(const Color& c) {
        glClearColor(c.rf(), c.gf(), c.bf(), c.af());
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void clear(const uint32_t c) {
        clear(Color(c));
    }

}
