#version 460 core

in vec2 v_UV;
in vec4 v_Color;

layout(location = 0) uniform sampler2D u_Texture;

layout(location = 0) out vec4 FragColor;

void main() {
    vec4 tex = texture(u_Texture, v_UV);
    FragColor = tex * v_Color;
}
