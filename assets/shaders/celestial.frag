#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 u_color;

void main() {
    float dist = distance(TexCoord, vec2(0.5));
    float alpha = 1.0 - smoothstep(0.4, 0.5, dist);
    if (alpha <= 0.0) discard;
    FragColor = vec4(u_color, alpha);
}