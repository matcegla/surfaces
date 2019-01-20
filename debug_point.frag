#version 330 core

in vec3 mid_pos;

out vec4 FragColor;

uniform vec3 color;

void main() {
    FragColor = vec4(color, 1);
}