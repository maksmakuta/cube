#version 460 core

in vec2 o_tex;
in vec3 o_norm;
flat in int o_id;

uniform sampler2DArray textures;

out vec4 out_color;

void main(){
    vec4 texColor = texture(textures, vec3(o_tex, o_id));
    float ambient = 0.5 + 0.3 * o_norm.y + 0.15 * abs(o_norm.x) + 0.15 * abs(o_norm.z);
    out_color = vec4(texColor.rgb * ambient, texColor.a);
}
