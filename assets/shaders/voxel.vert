#version 460 core

layout (location = 0) in uint a_PackedData;

out vec2 v_TexCoords;
flat out uint v_TexLayer;
out float v_AO;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    uint posX = (a_PackedData >> 0u)  & 0x1Fu;
    uint posY = (a_PackedData >> 5u)  & 0x1Fu;
    uint posZ = (a_PackedData >> 10u) & 0x1Fu;

    uint texU = (a_PackedData >> 15u) & 0x1Fu;
    uint texV = (a_PackedData >> 20u) & 0x1Fu;

    v_TexLayer = (a_PackedData >> 25u) & 0x1Fu;

    uint aoInt = (a_PackedData >> 30u) & 0x03u;

    vec3 localPos = vec3(float(posX), float(posY), float(posZ));
    v_TexCoords   = vec2(float(texU), float(texV));

    float aoTable[4] = float[](0.25, 0.50, 0.75, 1.00);
    v_AO = aoTable[aoInt];

    gl_Position = u_Projection * u_View * u_Model * vec4(localPos, 1.0);
}