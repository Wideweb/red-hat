#pragma once

#include "OpenGLRenderBatch.hpp"
#include "Render.hpp"
#include "Shader.hpp"

#include "glad/glad.h"
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Engine {

struct RenderBatch {
  public:
    Quad *quadPtr = nullptr;
    QuadVertices vetices;

    uint32_t textureId = 0;
    uint32_t indexCount = 0;

  public:
    ~RenderBatch() { delete[] this->quadPtr; }
};

struct RenderData {
    static const uint32_t maxQuads = 2000;
    static const uint32_t maxVertices = maxQuads * 4;
    static const uint32_t maxIndices = maxQuads * 6;

    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<VertexBuffer> quadBuffer;

    std::shared_ptr<Shader> shader;

    uint32_t indexCount = 0;

    uint32_t pointLightIndex = 0;
    uint32_t spotLightsIndex = 0;

    std::unordered_map<uint16_t, RenderBatch> batchMap;
};

class OpenGLRender : public Render {
  private:
    RenderData m_Data;

  public:
    OpenGLRender();
    virtual ~OpenGLRender();

    virtual void init() override;

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual void setViewport(uint16_t width, uint16_t height) override;
    virtual void setClearColor(float r, float g, float b, float a) override;
    virtual void clear() override;
    virtual void drawLines(std::shared_ptr<Shader> shader,
                           std::shared_ptr<VertexArray> vertexArray) override;
    virtual void drawQuad(const QuadVertices &vertices, const glm::vec4 &color,
                          const glm::mat4 &model,
                          const Material &material) override;
    virtual void
    drawTexture(const QuadVertices &vertices, const glm::mat4 &model,
                const glm::mat4 &textureModel, const Material &material,
                const std::shared_ptr<Texture> &texture, float alpha) override;
    virtual void
    drawTriangles(std::shared_ptr<Shader> shader,
                  std::shared_ptr<VertexArray> vertexArray) override;
    virtual void drawTexture(std::shared_ptr<Shader> shader,
                             std::shared_ptr<VertexArray> vertexArray,
                             std::shared_ptr<Texture> texture) override;
    virtual void drawTexture(std::shared_ptr<Shader> shader,
                             std::shared_ptr<VertexArray> vertexArray,
                             Texture *texture) override;

    virtual void addPointLight(const PointLight &light,
                               const glm::vec2 &position) override;
    virtual void addSpotLight(const SpotLight &light,
                              const glm::vec2 &position) override;
};

} // namespace Engine
