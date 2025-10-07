#version 460 core

in vec2 v_UV;
in vec4 v_Color;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main() {
    vec4 tex = texture(u_Texture, v_UV);
    if (v_UV.x < 0.0 || v_UV.y < 0.0) tex = vec4(1.0);
    FragColor = tex * v_Color;
}
