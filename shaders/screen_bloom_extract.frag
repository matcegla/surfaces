#version 330 core

in vec2 mtex;

out vec4 FragColor;

uniform sampler2D screen_texture;

void main() {
    vec3 color = vec3(texture(screen_texture, mtex));
    if (color.x + color.y + color.z < 1.55) {
        color = vec3(0);
    }
    FragColor = vec4(color, 1);
}