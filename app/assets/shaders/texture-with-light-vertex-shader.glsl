#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 model;
uniform mat4 texture_model;

out vec2 TexCoord;
out vec2 FragPos;
out vec3 Normal;

void main() {
    vec4 pos = model * vec4(a_position, 1.0f);
    gl_Position = pos;
    TexCoord = (texture_model * vec4(a_texCoord, 1.0f, 1.0f)).xy;
    FragPos = pos.xy;
    Normal = vec3(0, 0, -1);
}
