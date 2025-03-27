#include "cube/game/cube.hpp"

#include "cube/graphics/vertex.hpp"
#include "cube/graphics/gl/glad.h"
#include "cube/utils/utils.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

namespace cube::game {

    struct cubeVertex {
        glm::vec3 pos;
        glm::vec2 tex;
    };

    void Cube::init(){
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        m_shader.loadFilePair("../assets/shaders/","block");
        m_texture.load("../assets/textures/dirt.png");

        const graphics::Vertex cubeVertices[] = {
            graphics::pack({0,0,0},{0,0}),
            graphics::pack({0,0,1},{0,1}),
            graphics::pack({0,1,1},{1,1}),
            graphics::pack({0,0,0},{0,0}),
            graphics::pack({0,1,1},{1,1}),
            graphics::pack({0,1,0},{1,0}),

            graphics::pack({0,0,0},{0,0}),
            graphics::pack({0,0,1},{0,1}),
            graphics::pack({1,0,1},{1,1}),
            graphics::pack({0,0,0},{0,0}),
            graphics::pack({1,0,1},{1,1}),
            graphics::pack({1,0,0},{1,0}),

            graphics::pack({0,0,0},{0,0}),
            graphics::pack({0,1,0},{0,1}),
            graphics::pack({1,1,0},{1,1}),
            graphics::pack({0,0,0},{0,0}),
            graphics::pack({1,1,0},{1,1}),
            graphics::pack({1,0,0},{1,0}),

            graphics::pack({1,0,0},{0,0}),
            graphics::pack({1,0,1},{0,1}),
            graphics::pack({1,1,1},{1,1}),
            graphics::pack({1,0,0},{0,0}),
            graphics::pack({1,1,1},{1,1}),
            graphics::pack({1,1,0},{1,0}),

            graphics::pack({0,1,0},{0,0}),
            graphics::pack({0,1,1},{0,1}),
            graphics::pack({1,1,1},{1,1}),
            graphics::pack({0,1,0},{0,0}),
            graphics::pack({1,1,1},{1,1}),
            graphics::pack({1,1,0},{1,0}),

            graphics::pack({0,0,1},{0,0}),
            graphics::pack({0,1,1},{0,1}),
            graphics::pack({1,1,1},{1,1}),
            graphics::pack({0,0,1},{0,0}),
            graphics::pack({1,1,1},{1,1}),
            graphics::pack({1,0,1},{1,0}),
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(graphics::Vertex), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Cube::clear(){
        m_shader.clear();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Cube::draw(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_texture.handle());

        glUseProgram(m_shader.handle());

        m_shader.setMat4("proj",m_proj);
        m_shader.setMat4("model",glm::mat4(1.0f));
        m_shader.setMat4("view",m_camera.getMatrix());
        m_shader.setInt("atlas",0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Cube::update(const float delta){
        m_camera.update(m_direction,delta);
    }

    void Cube::onResize(const int w, const int h){
        glViewport(0, 0, w, h);
        const float aspect = static_cast<float>(w) / static_cast<float>(h);
        m_proj = glm::perspective(glm::radians(45.f),aspect,0.1f,100.f);
    }

    void Cube::onCursor(const float x, const float y){
        const auto curr = glm::vec2(x,-y);
        m_camera.rotate(curr - m_last);
        m_last = curr;
    }

    void Cube::onKey(const int key, const int action, const int mods){
        if (const auto d = getDir(key); d != 0) {
            utils::setBit(m_direction,d,action != GLFW_RELEASE);
        }
    }

    int Cube::getDir(const int key) {
        switch (key) {
            case GLFW_KEY_W: return render::CameraDirection::FORWARD;
            case GLFW_KEY_S: return render::CameraDirection::BACKWARD;
            case GLFW_KEY_A: return render::CameraDirection::LEFT;
            case GLFW_KEY_D: return render::CameraDirection::RIGHT;
            case GLFW_KEY_SPACE: return render::CameraDirection::UP;
            case GLFW_KEY_LEFT_SHIFT: return render::CameraDirection::DOWN;
            default: return 0;
        }
    }
}
