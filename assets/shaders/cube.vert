#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec3 offset;

uniform mat4 proj;
uniform mat4 view;

out vec3 f_col;

void main() {
    f_col = col;
    gl_Position = (proj * view) * vec4(pos + offset, 1.0);
}
