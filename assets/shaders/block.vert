#version 460 core

layout(location = 0) in uint vertex;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec2 f_tex;

void main() {
    float px = float( vertex & 0x000Fu);
    float pz = float((vertex >> 4) & 0x000Fu);
    float py = float((vertex >> 8) & 0x00FFu);
    float tu = float((vertex >> 16) & 0x000Fu) / 15.f;
    float tv = float((vertex >> 20) & 0x000Fu) / 15.f;

    f_tex = vec2(tu,tv);
    gl_Position = proj * model * view * vec4(px, py, pz, 1.0);
}
