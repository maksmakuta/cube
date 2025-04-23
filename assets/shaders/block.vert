#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 u_proj;
uniform mat4 u_model;
uniform mat4 u_view;

out vec2 f_tex;

void main() {
    gl_Position = u_proj * u_model * u_view * vec4(pos, 1.0);
    f_tex = tex;
}
