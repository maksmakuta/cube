#version 460 core

out vec4 FragColor;

in vec2 v_TexCoords;
in vec3 v_TintColor;
in float v_TexLayer;
in float v_AO;

uniform sampler2DArray u_TextureArray;

void main() {
    vec4 texColor = texture(u_TextureArray, vec3(v_TexCoords, v_TexLayer));

    if (texColor.a < 0.1) {
        discard;
    }

    FragColor = vec4(texColor.rgb * v_TintColor * v_AO, texColor.a);
}