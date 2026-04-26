#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;

layout (location = 0) out vec3 TexCoord;
layout (location = 1) out vec3 Normal;

uniform ivec3 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(model + aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = aNormal;
}