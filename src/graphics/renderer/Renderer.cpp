#include "cube/graphics/renderer/Renderer.hpp"

#include "cube/utils/AssetsUtils.hpp"
#include "glad/gl.h"

namespace cube {
    Renderer::Renderer() = default;
    Renderer::~Renderer() = default;

    void Renderer::onCreate() {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
             0.5f, -0.5f, 0.0f, // right
             0.0f,  0.5f, 0.0f  // top
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_shader.load(
            getAsset("/shaders/canvas.vert"),
            getAsset("/shaders/canvas.frag")
        );
    }

    void Renderer::onClear() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    void Renderer::onResize(const int w, const int h) {
        glViewport(0,0,w,h);
    }

    void Renderer::draw() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader.use();

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
