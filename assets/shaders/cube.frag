#version 460 core
in vec2 f_tex;
in float f_occlusion;

uniform sampler2D atlas;

out vec4 f_color;

void main() {
    vec4 col = texture(atlas, f_tex);
    f_color = vec4(col.rgb * f_occlusion, col.a);
}
