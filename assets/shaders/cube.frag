#version 450 core
out vec4 FragColor;

in vec3 TexCoord;
in float OverlayTexID;
in vec4 BiomeColor;
in vec3 Normal;
in float Frames;

uniform sampler2DArray texArray;
uniform float uTime;

void main() {
    float currentLayer = TexCoord.z;

    if (Frames > 1.0) {
        int currentFrameOffset = int(uTime * 4.0) % int(Frames);
        currentLayer += float(currentFrameOffset);
    }

    vec4 baseColor = texture(texArray, vec3(TexCoord.xy, currentLayer));

    if (OverlayTexID >= 0.0) {
        vec4 overlay = texture(texArray, vec3(TexCoord.xy, OverlayTexID));
        baseColor.rgb = mix(baseColor.rgb, overlay.rgb * BiomeColor.rgb, overlay.a);
        baseColor.a *= BiomeColor.a;
    } else {
        baseColor *= BiomeColor;
    }

    if (baseColor.a < 0.1) {
        discard;
    }

    float light = 1.0;
    if (Normal.y > 0.5) light = 1.0;
    else if (Normal.y < -0.5) light = 0.4;
    else if (abs(Normal.z) > 0.5) light = 0.8;
    else if (abs(Normal.x) > 0.5) light = 0.6;

    baseColor.rgb *= light;

    FragColor = baseColor;
}