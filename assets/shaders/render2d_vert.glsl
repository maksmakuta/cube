#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in uint col;

uniform mat4 proj;

out vec2 o_tex;
out vec4 o_col;

vec4 unpackColor(uint c) {
    return vec4(
        float((c >> 16) & 0xFFu) / 255.0,
        float((c >>  8) & 0xFFu) / 255.0,
        float((c >>  0) & 0xFFu) / 255.0,
        float((c >> 24) & 0xFFu) / 255.0
    );
}

void main() {
    o_tex = tex;
    o_col = unpackColor(col);
    gl_Position = proj * vec4(pos, 0.0, 1.0);
}