#version 460 core

in vec3 f_norm;
in vec3 f_tex;
in vec4 f_tint;

uniform sampler2DArray textures;

out vec4 o_color;

void main() {
    o_color = texture(textures, f_tex) * f_tint;
}
