#version 460 core

layout (location = 0) in uvec2 a_Data;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform vec3 u_model;
uniform float u_time;

out vec3 fs_Normal;
out vec3 fs_UV;
out vec4 fs_Color;
out float fs_Overlay;
out float fs_AO;
out vec3 fs_ViewPos;

const vec3 NORMALS[6] = vec3[](
    vec3(1,0,0), vec3(0,1,0), vec3(0,0,1),
    vec3(-1,0,0), vec3(0,-1,0), vec3(0,0,-1)
);

void main() {
    uint low  = a_Data.x;
    uint high = a_Data.y;

    vec3 localPos = vec3(low & 0x1Fu, (low >> 5u) & 0x1Fu, (low >> 10u) & 0x1Fu);
    vec4 viewPos = u_view * vec4(localPos + u_model, 1.0);
    fs_ViewPos = viewPos.xyz;
    gl_Position = u_proj * viewPos;

    fs_Normal = NORMALS[(low >> 15u) & 0x07u];

    float u = float((low >> 18u) & 0x03u);
    float v = float((low >> 20u) & 0x03u);
    uint ao = (low >> 22u) & 0x03u;
    fs_Overlay = float((low >> 24u) & 0x0Fu);
    float animSpeed = float((low >> 28u) & 0x07u);

    uint baseTexID = high & 0x0FFFu;
    uint frameCount = (high >> 12u) & 0x1Fu;

    float frameOffset = 0.0;
    if (frameCount > 1u) {
        frameOffset = floor(mod(u_time * (animSpeed + 1.0) * 5.0, float(frameCount)));
    }

    fs_UV = vec3(u, v, float(baseTexID) + frameOffset);

    float r = float((high >> 17u) & 0x1Fu) / 31.0;
    float g = float((high >> 22u) & 0x1Fu) / 31.0;
    float b = float((high >> 27u) & 0x1Fu) / 31.0;

    fs_Color = vec4(r, g, b, 1.0);
    fs_AO = float(ao);
}