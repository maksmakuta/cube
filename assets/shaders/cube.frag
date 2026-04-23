#version 460 core

layout (location = 0) in vec3 TexCoord;
layout (location = 1) in vec3 Normal;

layout (binding = 0) uniform sampler2DArray uTextureArray;

layout (location = 0) out vec4 FragColor;

const float gamma = 0.8;

void main() {
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(normalize(Normal), lightDir), 0.2);
    vec4 texColor = texture(uTextureArray, TexCoord);
    texColor.rgb = pow(texColor.rgb, vec3(gamma));
    if(texColor.a < 0.05) discard;
    FragColor = texColor * diff;
}