#version 330 core

in float mid_height;

out vec4 FragColor;

uniform float time;

void main() {
    float h = 8 - mid_height;
    float argument = (h - 4)/6;
    float intensity = argument * abs(argument) + 0.25;
    FragColor = mix(vec4(1,1,1,1), vec4(0,0,1,1), intensity);
}