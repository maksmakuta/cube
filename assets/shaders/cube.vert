#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in int ao;
layout(location = 3) in vec3 offset;

uniform mat4 proj;
uniform mat4 view;

out vec2 f_tex;
out float f_ao;

const float AO = 1.f / 6.f;

void main() {
    f_tex = tex;
    f_ao = float(ao) * 0.25f;
    gl_Position = (proj * view) * vec4(pos + offset, 1.0);
}
