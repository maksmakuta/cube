#version 460 core

layout(location = 0) in uvec2 data;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_sunIntensity = 1.0;

out vec3 f_norm;
out vec4 f_tex;
out vec4 f_tint;

void main() {
    uint x = data.x & 0x1Fu;
    uint y = (data.x >> 5u) & 0x1Fu;
    uint z = (data.x >> 10u) & 0x1Fu;

    uint normIdx = (data.x >> 15u) & 0x07u;

    float u = float((data.x >> 18u) & 0x0Fu) / 15.0;
    float v = float((data.x >> 22u) & 0x0Fu) / 15.0;

    uint texID = (data.x >> 26u) & 0x0Fu;
    uint overlay = (data.x >> 30u) | ((data.y & 0x03u) << 2u);

    float r = float((data.y >> 2u) & 0xFFu) / 255.0;
    float g = float((data.y >> 10u) & 0xFFu) / 255.0;
    float b = float((data.y >> 18u) & 0xFFu) / 255.0;

    float sun = float((data.y >> 26u) & 0x0Fu) / 15.0;
    float ao = float((data.y >> 30u) & 0x03u) / 3.0;


    vec3 normals[6] = vec3[](
        vec3( 1,  0,  0),
        vec3(-1,  0,  0),
        vec3( 0,  1,  0),
        vec3( 0, -1,  0),
        vec3( 0,  0,  1),
        vec3( 0,  0, -1)
    );
    f_norm = normals[min(normIdx, 5u)];

    f_tex = vec4(u, v, float(texID), float(overlay));

    float lightLevel = sun * u_sunIntensity * ao;
    f_tint = vec4(r * lightLevel, g * lightLevel, b * lightLevel, 1.0);

    vec3 pos = vec3(x, y, z);
    gl_Position = u_proj * u_view * u_model * vec4(pos, 1.0);
}
