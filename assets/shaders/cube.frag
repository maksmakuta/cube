#version 460 core

layout (location = 0) in vec3 vUV;   // uv.x, uv.y, layer
layout (location = 1) in vec3 vTint;
layout (binding = 0) uniform sampler2DArray uTextures;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(uTextures, vUV);

    // LAYER_GRASS_SIDE = 3.0
    // LAYER_GRASS_OVERLAY = 4.0
    if (vUV.z == 3.0) {
        vec4 overlay = texture(uTextures, vec3(vUV.xy, 4.0));

        // Apply tint to the overlay before mixing
        vec3 tintedOverlay = overlay.rgb * vTint;

        // mix(DirtBase, GreenOverlay, OverlayAlpha)
        texColor.rgb = mix(texColor.rgb, tintedOverlay, overlay.a);
    }
    // LAYER_GRASS_TOP = 2.0
    else if (vUV.z == 2.0) {
        texColor.rgb *= vTint;
    }

    if(texColor.a < 0.1) discard;
    FragColor = texColor;
}