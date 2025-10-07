#version 460 core

in vec2 v_UV;
in vec4 v_Color;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main() {
    //vec4 tex = texture(u_Texture, v_UV);
    FragColor = v_Color;
}
