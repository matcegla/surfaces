#version 330 core

layout (location = 0) in vec3 vertex_pos;

out float mid_height;
flat out vec3 mid_pos;
flat out vec3 mid_normal;

uniform float time;
uniform mat4 trans_pv;
uniform mat4 trans_model;

float heightAtPoint(vec3 pos) {
    float wavePresence = (sin((pos.x + pos.z + time)/16) + 1) / 2;
    float x = pos.z / 8 + pos.x / 32 + time / 2;
    float height = 4 * wavePresence * (sin(x) + sin(2*x) + sin(3*x));
//    float height = 0;
    return height;
}

vec3 posAtPoint(vec3 pos) {
    return pos + vec3(0, heightAtPoint(pos), 0);
}

void main() {

    float height = heightAtPoint(vertex_pos);
    vec3 pos = vertex_pos + vec3(0, height, 0);

    vec3 posRight = posAtPoint(vertex_pos + vec3(1, 0, 0));
    vec3 posFront = posAtPoint(vertex_pos + vec3(0, 0, 1));
    vec3 normal = normalize(cross(posFront, posRight));

    gl_Position = trans_pv * trans_model * vec4(pos, 1.0);
    mid_pos = pos;
    mid_normal = normal;
    mid_height = height;

}