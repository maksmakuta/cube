#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTex;
layout (location = 3) in vec4 aColor;

out vec3 TexCoord;
out float OverlayTexID;
out vec4 BiomeColor;
out vec3 Normal;

uniform ivec3 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoord = aTex.xyz;
    OverlayTexID = aTex.w;
    BiomeColor = aColor;
    Normal = aNormal;

    vec3 worldPos = aPos + vec3(model);
    gl_Position = projection * view * vec4(worldPos, 1.0);
}