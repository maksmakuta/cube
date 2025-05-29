#version 460 core
in vec2 f_tex;
in float f_ao;

uniform sampler2D atlas;

out vec4 f_color;

void main() {
    vec4 tex = texture(atlas, f_tex);
    f_color = vec4(tex.rgb * f_ao, tex.a);
}
