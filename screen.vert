#version 330 core

layout (location = 0) in vec2 vpos;
layout (location = 1) in vec2 vtex;

out vec2 mtex;

void main() {
    gl_Position = vec4(vpos, 0, 1);
    mtex = vtex;
}