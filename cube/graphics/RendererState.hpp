#ifndef CUBE_RENDERERSTATE_HPP
#define CUBE_RENDERERSTATE_HPP

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Color.hpp"
#include "Font.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureArray.hpp"

namespace cube {

    enum class State {
        Fill,
        Image,
        Stroke,
        Text
    };

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

    struct FillRenderState {
        Color       color       {0xFFFFFFFF};
    };

    struct ImageRenderState {
        Texture     texture     {};
        glm::vec4   texture_uv  {0.f, 0.f, 1.f, 1.f};
        glm::vec4   texture_box {0.f};
    };

    struct StrokeRenderState {
        Color       color       {0xFFFFFFFF};
        float       thickness   {1.f};
        JoinType    join        {JoinType::Miter};
        CapType     cap         {CapType::Flat};
        bool        is_loop     {false};
    };

    struct TextRenderState {
        Color       color       {0xFFFFFFFF};
        Font        font        {};
    };

    struct Vertex2D {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::uint col;
    };

    struct Vertex3D {
        glm::vec2 pos;
        glm::vec3 nor;
        glm::vec2 tex;
    };

    struct RendererGLState2D {
        uint32_t vao{0};
        uint32_t vbo{0};
        Shader shader;

        void init(int alloc = 64*1024);
        void flush(const std::vector<Vertex2D>&, int state) const;
        void release();

        void setProjection(const glm::mat4&) const;
        void setTexture(const Texture&) const;
    };

    struct RendererGLState3D {
        uint32_t vao{0};
        uint32_t vbo{0};
        Shader shader;
        TextureArray textures;

        void init(int alloc = 64*1024);
        void flush(const std::vector<Vertex2D>&) const;
        void release();

        void setProjection(const glm::mat4&) const;

    };

    struct RendererState2D {
        RendererGLState2D gl_state;
        FillRenderState fill_state;
        StrokeRenderState stroke_state;
        ImageRenderState image_state;
        TextRenderState text_state;
        glm::mat4 projection{1.f};
        std::vector<Vertex2D> vertices;
        State current_state{State::Fill};

        void init();
        void flush();
        void release();

        void resize(int w, int h);
    };

    struct RendererState3D {
        RendererGLState3D gl_state;
        glm::mat4 projection{1.f};
        std::vector<Vertex3D> vertices;

        void init();
        void flush();
        void release();

        void resize(int w, int h);
    };

    void clear(const Color&);
    void clear(uint32_t);

}

#endif //CUBE_RENDERERSTATE_HPP