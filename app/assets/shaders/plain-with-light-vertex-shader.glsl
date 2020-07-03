#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 model;

out vec4 Color;
out vec2 FragPos;
out vec3 Normal;

void main() {
    vec4 pos = model * vec4(a_position, 1.0f);
    gl_Position = pos;
    Color = a_color;
    FragPos = pos.xy;
    Normal = vec3(0, 0, -1);
}