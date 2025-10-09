#version 460 core

in vec2 v_UV;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform int u_IsText = 0; // 0 = normal texture, 1 = text

out vec4 FragColor;

void main() {
    if(v_UV.x < 0.f || v_UV.y < 0.f){
        FragColor = v_Color;
    }else{
        vec4 tex = texture(u_Texture, v_UV);
        if (u_IsText == 1)
            tex = vec4(1.0, 1.0, 1.0, tex.r);
        FragColor = tex * v_Color;
    }
}
