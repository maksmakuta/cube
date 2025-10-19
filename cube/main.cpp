#include <memory>
#include <sstream>
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "game/Generator.hpp"
#include "graphics/Color.hpp"
#include "graphics/Shader.hpp"
#include "graphics/TextureArray.hpp"
#include "graphics/TextureBuilder.hpp"
#include "models/Camera.hpp"
#include "models/world/Chunk.hpp"
#include "utils/AssetsPaths.hpp"
#include "utils/LambdaVisitor.hpp"
#include "window/Window.hpp"

struct ChunkRenderable {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t indices;
    glm::mat4 model;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec2 tex;
    uint32_t   id;
};

class CubeGame {
public:
    CubeGame() : m_renderable(0,0,0,0,{1.f}), m_window(std::make_shared<cube::Window>("Cube")) {
        m_renderable.model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
        m_shader.fromName("render3d");
        m_textures = cube::TextureBuilder()
                .setFilter(cube::TextureFilter::Nearest)
                .buildArray(cube::getTexture("/blocks/"));

        glEnable(GL_DEPTH_TEST);      // So closer fragments overwrite farther ones

        m_window->setCursor(2);
    }

    void run(){
        double time = 0.0;
        makeRenderable();
        std::cout << m_renderable.indices << std::endl;
        while(!m_window->isClosed()) {
            const double now = cube::getTime();
            const double delta = now - time;
            m_camera.update(delta);
            time = now;

            cube::update();
            while (m_window->isNextEvent()) {
                onEvent(m_window->getEvent());
            }

            onDraw();

            m_window->swapBuffers();

            std::stringstream ss;
            ss << "pos :" << glm::to_string(m_camera.getPosition()) << " , rot: " << glm::to_string(m_camera.getRotation());
            m_window->setTitle(ss.str());
        }
    }

protected:
    struct CubeMesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    CubeMesh createCubeMesh(const cube::BlockMetadata& meta)
    {
        CubeMesh mesh;

        static const glm::vec3 normals[6] = {
            { 0,  1,  0}, // top
            { 0, -1,  0}, // bottom
            { 0,  0,  1}, // front
            { 0,  0, -1}, // back
            { 1,  0,  0}, // right
            {-1,  0,  0}  // left
        };

        static const glm::vec3 offsets[6][4] = {
            {{0,1,0},{1,1,0},{1,1,1},{0,1,1}}, // top
            {{0,0,1},{1,0,1},{1,0,0},{0,0,0}}, // bottom
            {{0,0,1},{0,1,1},{1,1,1},{1,0,1}}, // front
            {{1,0,0},{1,1,0},{0,1,0},{0,0,0}}, // back
            {{1,0,1},{1,1,1},{1,1,0},{1,0,0}}, // right
            {{0,0,0},{0,1,0},{0,1,1},{0,0,1}}  // left
        };

        static const glm::vec2 uvs[4] = {
            {0,1},
            {0,0},
            {1,0},
            {1,1}
        };

        for(int f=0; f<6; ++f)
        {
            uint32_t startIndex = mesh.vertices.size();

            int texId;
            if(f==0) texId = meta.top;
            else if(f==1) texId = meta.bottom;
            else texId = meta.side;

            for(int i=0;i<4;i++)
            {
                Vertex v;
                v.pos = offsets[f][i];
                v.nor = normals[f];
                v.tex = uvs[i];
                v.id  = texId;
                mesh.vertices.push_back(v);
            }

            // two triangles per face
            mesh.indices.push_back(startIndex + 0);
            mesh.indices.push_back(startIndex + 1);
            mesh.indices.push_back(startIndex + 2);

            mesh.indices.push_back(startIndex + 2);
            mesh.indices.push_back(startIndex + 3);
            mesh.indices.push_back(startIndex + 0);
        }

        return mesh;
    }

    void onDraw() {
        cube::clear(0xFF808080);
        m_shader.use();
        m_shader.setMat4("proj", m_projection);
        m_shader.setMat4("view", m_camera.getMatrix());
        m_shader.setMat4("model", m_renderable.model);

        m_textures.bind();
        m_shader.setInt("textures", 0);

        glBindVertexArray(m_renderable.vao);
        glDrawElements(GL_TRIANGLES, m_renderable.indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void makeRenderable() {
        auto mesh = createCubeMesh(cube::getMetadata(m_block));
        m_renderable.indices = mesh.indices.size();
        glGenVertexArrays(1, &m_renderable.vao);
        glGenBuffers(1, &m_renderable.vbo);
        glGenBuffers(1, &m_renderable.ebo);

        glBindVertexArray(m_renderable.vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_renderable.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex),
                     mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderable.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t),
                     mesh.indices.data(), GL_STATIC_DRAW);

        // pos (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void*)offsetof(Vertex, pos));
        glEnableVertexAttribArray(0);

        // nor (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void*)offsetof(Vertex, nor));
        glEnableVertexAttribArray(1);

        // tex (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void*)offsetof(Vertex, tex));
        glEnableVertexAttribArray(2);

        // id (location = 3) — watch out, this must be integer type
        glVertexAttribIPointer(3, 1, GL_INT,
                               sizeof(Vertex), (void*)offsetof(Vertex, id));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void onEvent(const cube::Event& e) {
        std::visit(cube::LambdaVisitor{
            [this](const cube::ResizeEvent& re) {
                const auto wf = static_cast<float>(re.width);
                const auto hf = static_cast<float>(re.height);
                m_projection = glm::perspective(glm::radians(m_camera.getFOV()), wf / hf, 0.1f, 100.f);
                glViewport(0, 0, re.width, re.height);
            },
            [this](const cube::KeyEvent& ke) {
                if (ke.pressed && ke.key == cube::Key::Escape) {
                    m_window->close();
                }
                if (ke.pressed && ke.key == cube::Key::MouseLeft) {
                    m_window->setCursor(0);
                }
                if (ke.pressed && ke.key == cube::Key::MouseRight) {
                    m_window->setCursor(2);
                }
            },
            [](const cube::MouseEvent&) {},
            [](const cube::ScrollEvent&) {},
            [](const cube::InputEvent&) {}
        }, e);
        m_camera.onEvent(e);
    }

private:
    cube::Block m_block = cube::Block::Grass;
    cube::Shader m_shader;
    cube::TextureArray m_textures;
    cube::Camera m_camera{};
    glm::mat4 m_projection{1.f};
    ChunkRenderable m_renderable{};
    std::shared_ptr<cube::Window> m_window = nullptr;
};

int main(){
    std::make_unique<CubeGame>()->run();
    return 0;
}