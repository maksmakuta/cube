#version 460 core

in vec2 v_UV;
in float v_AO;
in float v_Light;
flat in uint v_BlockID;
flat in uint v_Normal;

out vec4 FragColor;

layout (binding = 0) uniform sampler2DArray u_TextureArray;

struct BlockMetadata {
    uint texTop;
    uint texSide;
    uint texBottom;
    uint frameCount;
};

layout (std430, binding = 1) buffer BlockPalette {
    BlockMetadata blocks[];
};

uniform float u_Time;
uniform float u_AnimSpeed = 10.0;

void main() {
    BlockMetadata data = blocks[v_BlockID];

    uint baseTex;
    if (v_Normal == 2)      baseTex = data.texTop;
    else if (v_Normal == 3) baseTex = data.texBottom;
    else                    baseTex = data.texSide;

    uint animationOffset = 0;
    if (data.frameCount > 1) {
        animationOffset = uint(u_Time * u_AnimSpeed) % data.frameCount;
    }

    uint finalLayer = baseTex + animationOffset;

    vec4 texColor = texture(u_TextureArray, vec3(v_UV, float(finalLayer)));

    if (texColor.a < 0.1) {
        discard;
    }

    float brightness = (v_Light * v_AO) + 0.05;

    FragColor = vec4(texColor.rgb * brightness, texColor.a);
}