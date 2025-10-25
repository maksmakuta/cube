#version 460 core

in vec2 o_tex;
in vec3 o_norm;
in float o_ao;
flat in int o_id;

uniform sampler2DArray textures;

out vec4 out_color;

void main(){
    vec4 texColor = texture(textures, vec3(o_tex, o_id));
    texColor.rgb *= o_ao;
    out_color = texColor;
}
