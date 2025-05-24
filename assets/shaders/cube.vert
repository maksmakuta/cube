#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in int ao;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 f_tex;
out float f_occlusion;

void main() {
    f_occlusion = 0.25f + 0.25f * float(ao);
    f_tex = tex;
    gl_Position = (proj * view * model) * vec4(pos, 1.0);
}
