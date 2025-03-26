#version 460 core
in vec2 f_tex;

uniform sampler2D atlas;

out vec4 color;

void main() {
    color = texture(atlas,f_tex);
}
