#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 f_tex;
out vec3 f_norm;
out vec3 f_pos;

void main() {
    f_tex = tex;
    f_norm = nor;
    f_pos = vec3(model * vec4(pos, 1.0));
    gl_Position = (proj * view * model) * vec4(pos, 1.0);
}
