#version 460 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in uint a_Color;

layout(location = 0) uniform mat4 u_Projection;

out vec2 v_UV;
out vec4 v_Color;

vec4 unpackColor(uint c) {
    return vec4(
        float((c >> 16) & 0xFFu) / 255.0,
        float((c >>  8) & 0xFFu) / 255.0,
        float((c >>  0) & 0xFFu) / 255.0,
        float((c >> 24) & 0xFFu) / 255.0
    );
}

void main() {
    v_UV = a_UV;
    v_Color = unpackColor(a_Color);
    gl_Position = u_Projection * vec4(a_Pos, 0.0, 1.0);
}