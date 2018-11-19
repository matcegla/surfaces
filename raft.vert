#version 330 core

layout (location = 0) in vec3 vertex_pos;

uniform mat4 trans_pv;
uniform mat4 trans_model;

void main() {
    gl_Position = trans_pv * trans_model * vec4(vertex_pos, 1.0);
}