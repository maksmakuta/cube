#version 460 core

in vec3  fs_Normal;
in vec3  fs_UV;
in vec4  fs_Color;
in float fs_Overlay;
in float fs_AO;
in vec3  fs_ViewPos;

layout(binding = 0) uniform sampler2DArray u_textures;

uniform vec3  u_sunDir;
uniform float u_sunAngle;
uniform vec3  u_skyColor;
uniform float u_minFog;
uniform float u_maxFog;

out vec4 FragColor;

void main() {
    vec3 nightAmbientColor = vec3(0.1, 0.15, 0.3);
    vec3 dayAmbientColor   = vec3(0.4, 0.4, 0.45);

    float aoLerp = fs_AO / 3.0;
    float aoFactor = mix(0.4, 1.0, pow(aoLerp, 1.5));

    vec4 texColor = texture(u_textures, fs_UV);
    if(texColor.a < 0.1) discard;

    float sunHeight = u_sunDir.y;
    float daySmooth = smoothstep(-0.2, 0.2, sunHeight);
    float minAmbient = 0.085;

    vec3 ambient = mix(nightAmbientColor, dayAmbientColor, daySmooth);
    float sunDiff = dot(fs_Normal, u_sunDir) * 0.5 + 0.5;
    sunDiff = pow(sunDiff, 2.0);
    vec3 sunColor = vec3(1.0, 0.9, 0.8) * sunDiff * daySmooth;
    float moonDiff = max(dot(fs_Normal, -u_sunDir), 0.0);
    vec3 moonColor = vec3(0.3, 0.4, 0.7) * moonDiff * (1.0 - daySmooth);
    vec3 totalLight = (sunColor + moonColor + ambient) + minAmbient;
    totalLight *= fs_Color.a;
    vec3 finalRGB;

    if (fs_Overlay > 0.0) {
        vec4 overlayColor = texture(u_textures, vec3(fs_UV.xy, fs_Overlay));
        vec3 baseLayer = texColor.rgb * totalLight;
        vec3 tintedOverlay = overlayColor.rgb * fs_Color.rgb * totalLight;
        finalRGB = mix(baseLayer, tintedOverlay, overlayColor.a) * aoFactor;
    } else {
        finalRGB = texColor.rgb * fs_Color.rgb * totalLight * aoFactor;
    }

    float dist = length(fs_ViewPos);
    float fogFactor = clamp((dist - u_minFog) / (u_maxFog - u_minFog), 0.0, 1.0);
    vec3 finalWithFog = mix(finalRGB, u_skyColor, fogFactor);

    FragColor = vec4(finalWithFog, texColor.a);
}