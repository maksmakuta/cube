#version 460 core
in vec3 f_col;
out vec4 f_color;

void main() {
    f_color = vec4(f_col, 1.0);
}
