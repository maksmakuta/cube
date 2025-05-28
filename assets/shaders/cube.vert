#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 offset;

uniform mat4 proj;
uniform mat4 view;

out vec2 f_tex;

void main() {
    f_tex = tex;
    gl_Position = (proj * view) * vec4(pos + offset, 1.0);
}
