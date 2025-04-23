#version 460 core

in vec2 f_tex;

uniform sampler2D u_atlas;

out vec4 o_color;

void main() {
    o_color = texture2D(u_atlas,f_tex);
}
