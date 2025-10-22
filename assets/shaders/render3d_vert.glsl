#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;
layout(location = 3) in int id;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 o_tex;
out vec3 o_norm;
flat out int o_id;

void main(){
    gl_Position = proj * view * model * vec4(pos, 1.0);
    o_tex = tex;
    o_id = id;
    o_norm = nor;
}