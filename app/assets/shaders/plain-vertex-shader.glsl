#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 model;

out vec4 Color;

void main() {
    vec4 pos = model * vec4(a_position, 1.0f);
    gl_Position = pos;
    Color = a_color;
}