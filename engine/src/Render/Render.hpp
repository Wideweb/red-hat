#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>

namespace Engine {

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct QuadVertex {
    glm::vec3 position;
    glm::vec2 texCoord;

    friend std::ostream &operator<<(std::ostream &out, const QuadVertex &v) {
        out << v.texCoord.x << " ";
        out << v.texCoord.y << std::endl;
        return out;
    }
};

typedef std::array<QuadVertex, 4> QuadVertices;

struct Quad {
    glm::vec4 color;
    glm::mat4 model;
    glm::mat4 textureModel;
    Material material;
    float texIndex;
};

struct PointLight {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

class Render {
  public:
    virtual ~Render() = default;

    virtual void init() = 0;

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    virtual void setViewport(uint16_t width, uint16_t height) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;
    virtual void drawQuad(const QuadVertices &vertices, const glm::vec4 &color,
                          const glm::mat4 &model, const Material &material) = 0;
    virtual void
    drawTexture(const QuadVertices &vertices, const glm::mat4 &model,
                const glm::mat4 &textureModel, const Material &material,
                const std::shared_ptr<Texture> &texture, float alpha) = 0;
    virtual void drawLines(std::shared_ptr<Shader> shader,
                           std::shared_ptr<VertexArray> vertexArray) = 0;
    virtual void drawTriangles(std::shared_ptr<Shader> shader,
                               std::shared_ptr<VertexArray> vertexArray) = 0;
    virtual void drawTexture(std::shared_ptr<Shader> shader,
                             std::shared_ptr<VertexArray> vertexArray,
                             std::shared_ptr<Texture> texture) = 0;
    virtual void drawTexture(std::shared_ptr<Shader> shader,
                             std::shared_ptr<VertexArray> vertexArray,
                             Texture *texture) = 0;

    virtual void addPointLight(const PointLight &light,
                               const glm::vec2 &position) = 0;
    virtual void addSpotLight(const SpotLight &light,
                              const glm::vec2 &position) = 0;

    static Render *create();
};

} // namespace Engine
