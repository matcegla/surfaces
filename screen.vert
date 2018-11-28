#version 330 core

layout (location = 0) in vec2 vpos;
layout (location = 1) in vec2 vtex;

out vec2 mtex;

uniform vec2 pos1;
uniform vec2 pos2;

void main() {
    vec2 vpos01 = (vpos + 1) / 2;
    vec2 pos01 = vpos01 * (pos2 - pos1) + pos1;
    vec2 pos = pos01 * 2 - 1;
    gl_Position = vec4(pos, 0, 1);

    mtex = vtex;
}