#version 330 core

layout (location = 0) in vec3 vertex_pos;

out float mid_height;

uniform float time;
uniform mat4 trans_pv;
uniform mat4 trans_model;

void main() {
    float diagonal = vertex_pos.x + vertex_pos.z;
    float wavePresence = (sin(diagonal/16) + 1) / 2;
    float waveHeight = 8 * (sin(time + vertex_pos.z/4) + 1) / 2;
    float height = wavePresence * waveHeight;

    vec3 updatedPos = vec3(vertex_pos.x, height, vertex_pos.z);
    gl_Position = trans_pv * trans_model * vec4(updatedPos, 1.0);
    mid_height = height;
}