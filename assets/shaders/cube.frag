#version 460 core

in vec3 f_norm;
in vec4 f_tex;
in vec4 f_tint;

uniform sampler2DArray textures;

out vec4 o_color;

void main() {
    vec4 base = texture(textures, f_tex.xyz);

    if (f_tex.w < 0.0) {
        base.rgb *= f_tint.rgb;
    }

    if (f_tex.w >= 0.0) {
        vec4 overlay = texture(textures, vec3(f_tex.xy, f_tex.w));
        overlay.rgb *= f_tint.rgb;
        base.rgb = mix(base.rgb, overlay.rgb, overlay.a);
    }

    o_color = base;
}
