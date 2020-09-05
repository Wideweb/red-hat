#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

layout(location = 2) in vec4 a_color;
layout(location = 3) in mat4 a_model;
layout(location = 7) in mat4 a_texModel;
layout(location = 11) in vec3 a_ambient;
layout(location = 12) in vec3 a_diffuse;
layout(location = 13) in vec3 a_specular;
layout(location = 14) in float a_shininess;
layout(location = 15) in float a_texIndex;

out vec2 FragPos;
out vec2 TexCoord;
out vec4 Color;
out vec3 Normal;
out Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}
material;
out float TextureIndex;

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

    TextureIndex = a_texIndex;
    TexCoord = (a_texModel * vec4(a_texCoord, 1.0f, 1.0f)).xy;
}