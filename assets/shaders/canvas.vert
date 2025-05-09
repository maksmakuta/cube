#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec4 col;

uniform mat4 proj;

out vec2 f_tex;
out vec4 f_col;

void main() {
    gl_Position = proj * vec4(pos, 0.0, 1.0);
    f_tex = tex;
    f_col = col;
}
