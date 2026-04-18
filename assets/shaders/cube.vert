#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aUV;
layout (location = 2) in vec3 aTint;

// Outputs to Fragment Shader
layout (location = 0) out vec3 vUV;   // x, y = UV, z = Layer
layout (location = 1) out vec3 vTint;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Standard MVP transformation
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass data to fragment shader
    vUV = vec3(aUV.x, 1.0 - aUV.y, aUV.z);
    vTint = aTint;
}