#version 330 core

in vec3 mid_pos;

out vec4 FragColor;

void main() {
    vec3 light_wood = vec3(0.651, 0.502, 0.3922);
    vec3 dark_wood = vec3(0.5216, 0.3686, 0.2588);
    FragColor = vec4(mix(light_wood, dark_wood, (sin(mid_pos.z * 50) + 1) / 2), 1);
}