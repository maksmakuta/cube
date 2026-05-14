#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_view;
uniform mat4 u_proj;
uniform vec3 u_pos;
uniform float u_size;

out vec2 TexCoord;

void main() {
    TexCoord = aTexCoord;
    vec4 viewPos = u_view * vec4(u_pos, 1.0);
    viewPos.xy += aPos.xy * u_size;
    gl_Position = u_proj * viewPos;
}