#version 460 core

in vec2 o_tex;
in vec4 o_col;

uniform sampler2D image;
uniform int mode;

out vec4 out_color;

void main() {
    if(mode == 0 || mode == 2){
        out_color = o_col;
    }else if(mode == 1){
        out_color = texture(image,o_tex);
    }else{
        out_color = vec4(o_col.rgb, o_col.a * texture(image,o_tex).r);
    }

}
