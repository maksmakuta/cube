#version 460 core

layout (location = 0) in vec3 TexCoord;
layout (location = 1) in vec3 Normal;

layout (binding = 0) uniform sampler2DArray uTextureArray;
uniform vec3 uOverlay = vec3(0.16, 0.75, 0.15);

layout (location = 0) out vec4 FragColor;

const float gamma = 0.8;

void main() {
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    float ambient = 0.2;
    float lightIntensity = diff + ambient;

    int blockLayer = int(TexCoord.z + 0.5);
    vec4 texColor = texture(uTextureArray, TexCoord);

    if(blockLayer == 2) {
        vec4 overlayColor = texture(uTextureArray, vec3(TexCoord.xy, 0));
        texColor.rgb += overlayColor.rgb * uOverlay;
    }
    else if(blockLayer == 1) {
        texColor.rgb *= uOverlay;
    }

    if(texColor.a < 0.1) discard;

    texColor.rgb = pow(texColor.rgb, vec3(gamma));
    FragColor = vec4(texColor.rgb * lightIntensity, texColor.a);
}