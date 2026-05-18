#version 460 core

layout (location = 0) in uvec2 a_PackedData;

out vec2 v_TexCoords;
out vec3 v_TintColor;
out float v_TexLayer;
out float v_AO;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    uint posX = (a_PackedData.x >> 0u)  & 0x1Fu;
    uint posY = (a_PackedData.x >> 5u)  & 0x1Fu;
    uint posZ = (a_PackedData.x >> 10u) & 0x1Fu;

    uint texU = (a_PackedData.x >> 15u) & 0x1Fu;
    uint texV = (a_PackedData.x >> 20u) & 0x1Fu;

    v_TexLayer = float((a_PackedData.y >> 0u) & 0xFFu);
    uint aoInt  = (a_PackedData.y >> 8u) & 0x03u;

    vec3 localPos = vec3(float(posX), float(posY), float(posZ));
    v_TexCoords   = vec2(float(texU), float(texV));

    float aoTable[4] = float[](0.25, 0.50, 0.75, 1.00);
    v_AO = aoTable[aoInt];

    uint tint565 = (a_PackedData.y >> 10u) & 0xFFFFu;

    v_TintColor.r = float((tint565 >> 11u) & 31u) / 31.0;
    v_TintColor.g = float((tint565 >> 5u)  & 63u) / 63.0;
    v_TintColor.b = float(tint565 & 31u) / 31.0;

    gl_Position = u_Projection * u_View * u_Model * vec4(localPos, 1.0);
}