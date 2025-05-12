#version 330 core
in vec2 f_tex;
in vec3 f_norm;
in vec3 f_pos;

uniform vec3 lightPos;
uniform sampler2D atlas;

out vec4 f_color;

void main() {

    vec3 lightDir = normalize(lightPos - f_pos);
    float diff = max(dot(f_norm, lightDir), 0.1);
    vec3 diffuse = diff * texture(atlas,f_tex).rgb;

    f_color = vec4(diffuse, 1.0);
}
