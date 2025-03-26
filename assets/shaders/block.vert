#version 460 core

//layout(location = 0) in uint vertex;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec2 f_tex;

void main() {
//    float px = float(vertex & 0xFu);
//    float pz = float((vertex >> 4) & 0xFu);
//    float py = float((vertex >> 8) & 0xFFu);
//    float tu = float((vertex >> 16) & 0xFu);
//    float tv = float((vertex >> 20) & 0xFu);

    f_tex = tex;
    gl_Position = proj * model * view * vec4(pos, 1.0);
}
