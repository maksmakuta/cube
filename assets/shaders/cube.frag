#version 330 core

in vec2 f_tex;

uniform sampler2D image;

out vec4 f_color;

void main() {
    f_color = texture(image,f_tex);
}
