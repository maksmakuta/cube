#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uv;
layout(location = 3) in float id;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 f_norm;
out vec3 f_tex;

void main() {
    f_norm = norm;
    f_tex = vec3(uv,id);
    gl_Position = u_proj * u_view * u_model * vec4(pos, 1.0);
}
