#ifndef CUBE_TEXTUREPARAMS_HPP
#define CUBE_TEXTUREPARAMS_HPP

#include "glad/gl.h"

namespace cube {

    enum class TextureFormat : GLenum {
        RGBA8 = GL_RGBA8,
        RGB8  = GL_RGB8,
        RG8   = GL_RG8,
        R8    = GL_RED,
        DEPTH = GL_DEPTH_COMPONENT24,
    };

    enum class TextureFilter : GLint {
        Nearest         = GL_NEAREST,
        Linear          = GL_LINEAR,
        NearestNearest  = GL_NEAREST_MIPMAP_NEAREST,
        NearestLinear   = GL_NEAREST_MIPMAP_LINEAR,
        LinearLinear    = GL_LINEAR_MIPMAP_LINEAR,
        LinearNearest   = GL_LINEAR_MIPMAP_NEAREST,
    };

    enum class TextureWrap : GLint {
        ClampEdge   = GL_CLAMP_TO_EDGE,
        ClampBorder = GL_CLAMP_TO_BORDER,
        Repeat      = GL_REPEAT,
        Mirror      = GL_MIRRORED_REPEAT,
    };

}

#endif //CUBE_TEXTUREPARAMS_HPP