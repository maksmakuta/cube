#version 330 core
in vec4 f_col;
in vec2 f_tex;

uniform int type;
uniform sampler2D image;

out vec4 f_color;

void main() {
    if (type == 1){
        f_color = f_col;
    }else if (type == 2){
        f_color = texture(image,f_tex);
    }else if (type == 3){
        f_color = vec4(f_col.rgb, f_col.a * texture(image,f_tex).r);
    }else{
        discard;
    }
}
