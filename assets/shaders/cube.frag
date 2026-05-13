#version 460 core

in vec3  fs_Normal;
in vec3  fs_UV;
in vec4  fs_Color;
in float fs_Overlay;

out vec4 FragColor;

layout(binding = 0) uniform sampler2DArray u_textures;

uniform vec3  u_sunDir;
uniform float u_sunAngle;

void main() {
    vec3 noonColor   = vec3(1.0, 1.0, 0.9);
    vec3 sunsetColor = vec3(1.0, 0.5, 0.2);
    vec3 moonColor   = vec3(0.2, 0.3, 0.5);

    vec4 texColor = texture(u_textures, fs_UV);
    if(texColor.a < 0.1) discard;

    float sunHeight = max(u_sunDir.y, 0.0);

    vec3 sunColor = mix(sunsetColor, noonColor, sunHeight);
    if (u_sunDir.y < 0.0) sunColor = moonColor;

    float diffuse = max(dot(fs_Normal, u_sunDir), 0.0);

    float ambientStrength = mix(0.1, 0.4, sunHeight);

    float dayFactor = clamp(sunHeight * 5.0, 0.0, 1.0);

    vec3 light = (diffuse * sunColor * dayFactor) + (ambientStrength * noonColor);
    light *= fs_Color.a;

    vec3 finalRGB = texColor.rgb * fs_Color.rgb * light;

    if (fs_Overlay > 0.0) {
        vec4 overlayColor = texture(u_textures, vec3(fs_UV.xy, fs_Overlay));
        finalRGB = mix(finalRGB, overlayColor.rgb * light, overlayColor.a);
    }

    float dist = length(gl_FragCoord.z / gl_FragCoord.w);
    float fogFactor = clamp((dist - 64.0) / (128.0 - 64.0), 0.0, 1.0);
    vec3 skyColor = mix(vec3(0.02, 0.02, 0.1), vec3(0.5, 0.7, 1.0), sunHeight);

    FragColor.rgb = mix(FragColor.rgb, skyColor, fogFactor);

    FragColor = vec4(finalRGB, texColor.a);
}