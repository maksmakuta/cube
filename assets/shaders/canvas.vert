#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec4 col;

uniform mat4 proj;

out vec4 f_col;
out vec2 f_tex;

void main() {
    f_col = col;
    f_tex = tex;
    gl_Position = proj * vec4(pos, 0.0, 1.0);
}
