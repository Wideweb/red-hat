#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in mat4 a_model;
layout(location = 6) in vec3 a_ambient;
layout(location = 7) in vec3 a_diffuse;
layout(location = 8) in vec3 a_specular;
layout(location = 9) in float a_shininess;

out vec4 Color;
out vec2 FragPos;
out vec3 Normal;
out Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}
material;

void main() {
    vec4 pos = a_model * vec4(a_position, 1.0f);
    gl_Position = pos;
    Color = a_color;
    FragPos = pos.xy;
    Normal = vec3(0, 0, -1);

    material.ambient = a_ambient;
    material.diffuse = a_diffuse;
    material.specular = a_specular;
    material.shininess = a_shininess;
}