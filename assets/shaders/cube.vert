#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 f_tex;

void main() {
    f_tex = tex;
    gl_Position = (proj * view * model) * vec4(pos, 1.0);
}
