#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in uint col;

uniform mat4 proj;

out vec4 f_col;
out vec2 f_tex;

void main() {
    f_col = vec4(
        float((col >> 16) & 0xFFu) / 255.f,
        float((col >> 8 ) & 0xFFu) / 255.f,
        float((col >> 0 ) & 0xFFu) / 255.f,
        float((col >> 24) & 0xFFu) / 255.f
    );
    f_tex = tex;
    gl_Position = proj * vec4(pos, 0.0, 1.0);
}
