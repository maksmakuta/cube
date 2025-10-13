#version 460 core

in vec2 o_tex;
flat in int o_id;

uniform sampler2DArray textures;

out vec4 out_color;

void main(){
    out_color = texture(textures,vec3(o_tex, o_id));
}
