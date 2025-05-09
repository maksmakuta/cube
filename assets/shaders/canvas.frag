#version 460 core

in vec2 f_tex;
in vec4 f_col;

uniform int type = 1;
uniform sampler2D image;

out vec4 color;

void main() {
    color = vec4(0,0,0,1);

/*    if(type == 1){
        color = f_col;
    }else if(type == 2){
        color = texture(image,f_tex);
    }else if(type == 2){
        color = vec4( vec3(texture(image,f_tex).r), 1.0);
    }else {
        discard;
    }*/
}
