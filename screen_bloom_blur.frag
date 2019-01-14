#version 330 core

in vec2 mtex;

out vec4 FragColor;

uniform sampler2D screen_texture;

vec3 extractColor(vec2 texpos) {
    vec3 color = vec3(texture(screen_texture, texpos));
    if (color.x + color.y + color.z < 1.55) {
        color = vec3(0);
    }
    return color;
}

const float offset = 1.0 / 100.0;
const vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
);
const float kernel[9] = float[](
    0.2f, 0.2f, 0.2f,
    0.2f, 0.4f, 0.2f,
    0.2f, 0.2f, 0.2f
);

vec3 kernelled() {
    vec3 color = vec3(0);
    for (int i=0; i<9; i++)
        color += kernel[i] * vec3(extractColor(mtex + offsets[i]));
    return color;
}

void main() {
    vec3 color = kernelled();
    FragColor = vec4(color, 1);
}