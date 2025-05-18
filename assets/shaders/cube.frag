#version 330 core
in vec2 f_tex;

uniform sampler2D atlas;

out vec4 f_color;

void main() {
    f_color = vec4(1.0);//texture(atlas,f_tex);
}
