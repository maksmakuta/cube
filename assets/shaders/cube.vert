#version 460 core
layout (location = 0) in uint packedData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float u_Time;

out vec2 v_UV;
out float v_AO;
out float v_Light;
flat out uint v_BlockID;

void main() {
    uint x =  packedData        & 0x0Fu;
    uint y = (packedData >> 4)  & 0x0Fu;
    uint z = (packedData >> 8)  & 0x0Fu;

    uint ao    = (packedData >> 12) & 0x03u;
    uint u     = (packedData >> 14) & 0x01u;
    uint v     = (packedData >> 15) & 0x01u;
    uint norm  = (packedData >> 16) & 0x07u;

    uint light = (packedData >> 19) & 0x0Fu;
    uint sway  = (packedData >> 23) & 0x01u;
    v_BlockID  = (packedData >> 24) & 0xFFu;

    v_UV = vec2(float(u), float(v));
    v_AO = 0.4 + (float(ao) / 3.0) * 0.6;
    v_Light = float(light) / 15.0;

    vec4 localPos = vec4(float(x), float(y), float(z), 1.0);

    if (sway == 1u && v > 0u) {
        float wave = sin(u_Time * 2.0 + (model[3].x + localPos.x) + (model[3].z + localPos.z)) * 0.15;
        localPos.x += wave;
        localPos.z += wave;
    }

    gl_Position = projection * view * model * localPos;
}