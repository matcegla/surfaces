#version 330 core

struct Sun {
    vec3 pos;
    vec3 color;
};

in float mid_height;
flat in vec3 mid_pos;
flat in vec3 mid_normal;

out vec4 FragColor;

uniform Sun sun;
uniform vec3 view_pos;
uniform float time;

void main() {
    float x = mid_pos.x;
    float y = mid_pos.z;
    float q1 = sin(x*sin(pow(y,5)))+sin(sin(pow(x,3))*y);
    float q = q1 + sin(4 * q1 * time);
    vec3 light_sea_green = vec3(0.125, 0.698, 0.667);
    vec3 material_color = light_sea_green + 0.1 * vec3(sin(q), sin(2*q), sin(3*q));

    vec3 norm = normalize(mid_normal);
    vec3 light_dir = normalize(sun.pos - mid_pos);
    float diff = max(dot(norm, light_dir), 0);
    float specular_strenght = 1;
    vec3 view_dir = normalize(view_pos - mid_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 16);

    vec3 ambient = 0.1 * sun.color;
    vec3 diffuse = diff * sun.color;
    vec3 specular = specular_strenght * spec * vec3(233, 189, 21) / 255;

    vec3 total = material_color * (ambient + diffuse) + specular * (1 + sin(q)) / 2;
    FragColor = vec4(total, 1);
}