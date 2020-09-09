#include "OpenGLRender.hpp"
#include "Debug.hpp"
#include "File.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Engine {

OpenGLRender::OpenGLRender() {}

OpenGLRender::~OpenGLRender() {}

void OpenGLRender::init() {
    m_Data.vertexArray.reset(VertexArray::create());

    m_Data.vertexBuffer.reset(VertexBuffer::create(sizeof(QuadVertices)));
    m_Data.vertexBuffer->setLayout({{ShaderDataType::Float3, "a_position"},
                                    {ShaderDataType::Float2, "a_texCoord"}});
    m_Data.vertexArray->addVertexBuffer(m_Data.vertexBuffer);

    m_Data.quadBuffer.reset(
        VertexBuffer::create(m_Data.maxQuads * sizeof(Quad)));
    m_Data.quadBuffer->setLayout(
        {{ShaderDataType::Float4, "a_color", true},
         {ShaderDataType::Float4, "a_model1", true},
         {ShaderDataType::Float4, "a_model2", true},
         {ShaderDataType::Float4, "a_model3", true},
         {ShaderDataType::Float4, "a_model4", true},
         {ShaderDataType::Float4, "a_tex_model1", true},
         {ShaderDataType::Float4, "a_tex_model2", true},
         {ShaderDataType::Float4, "a_tex_model3", true},
         {ShaderDataType::Float4, "a_tex_model4", true},
         {ShaderDataType::Float3, "a_material_ambient", true},
         {ShaderDataType::Float3, "a_material_diffuse", true},
         {ShaderDataType::Float3, "a_material_specular", true},
         {ShaderDataType::Float, "a_material_shininess", true},
         {ShaderDataType::Float, "a_texIndex", true}});
    m_Data.vertexArray->addVertexBuffer(m_Data.quadBuffer);

    std::vector<uint32_t> indices(m_Data.maxIndices);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < m_Data.maxIndices; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 1;
        indices[i + 4] = offset + 2;
        indices[i + 5] = offset + 3;

        offset += 4;
    }

    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::create(indices));
    m_Data.vertexArray->setIndexBuffer(indexBuffer);

    std::string vertexSrc = File::read("./assets/shaders/vertex-shader.glsl");
    std::string fragmentSrc =
        File::read("./assets/shaders/fragment-shader.glsl");

    m_Data.shader.reset(Shader::create(vertexSrc, fragmentSrc));
}

void OpenGLRender::beginScene() {
    m_Data.indexCount = 0;
    m_Data.pointLightIndex = 0;
    m_Data.spotLightsIndex = 0;
    m_Data.batchMap.clear();
}

void OpenGLRender::endScene() {
    m_Data.shader->bind();
    m_Data.vertexArray->bind();

    m_Data.shader->setInt("spotLightsNumber", m_Data.spotLightsIndex);
    m_Data.shader->setInt("pointLightsNumber", m_Data.pointLightIndex);

    for (const auto &kv : m_Data.batchMap) {
        auto& batch = kv.second;
        m_Data.vertexBuffer->setData(batch.vetices,
                                     m_Data.indexCount * sizeof(QuadVertices));
        m_Data.quadBuffer->setData(m_Data.quadPtr,
                                   m_Data.indexCount * sizeof(Quad));

        glDrawElements(GL_TRIANGLES, m_Data.indexCount * 6, GL_UNSIGNED_INT,
                       nullptr);
    }

    GL_CHECK();
}

void OpenGLRender::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    GL_CHECK();
}

void OpenGLRender::setViewport(uint16_t width, uint16_t height) {
    glViewport(0, 0, width, height);
    GL_CHECK();
}

void OpenGLRender::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
    GL_CHECK();
}

void OpenGLRender::drawQuad(const QuadVertices &vertices,
                            const glm::vec4 &color, const glm::mat4 &model,
                            const Material &material) {
    // memcpy(m_Data.veticesPtr + m_Data.indexCount, vertices.data(),
    //        sizeof(QuadVertices));

    // m_Data.quadPtr[m_Data.indexCount].color = color;
    // m_Data.quadPtr[m_Data.indexCount].model = model;
    // m_Data.quadPtr[m_Data.indexCount].material = material;
    // m_Data.quadPtr[m_Data.indexCount].texIndex = -1;

    // m_Data.indexCount++;
}

void OpenGLRender::drawTexture(const QuadVertices &vertices,
                               const glm::mat4 &model,
                               const glm::mat4 &textureModel,
                               const Material &material,
                               const std::shared_ptr<Texture> &texture,
                               float alpha) {
    auto batchIt = m_Data.batchMap.find(texture->getId());
    if (batchIt == m_Data.batchMap.end()) {
        auto &batch = m_Data.batchMap[texture->getId()];
        batch.quadPtr = new Quad[m_Data.maxQuads];
        batch.vetices = vertices;
        batch.textureId = texture->getId();

        // glActiveTexture(GL_TEXTURE0 + m_Data.texIndex);
        // texture->bind();

        // auto iStr = std::to_string(m_Data.texIndex);
        // m_Data.shader->setInt("textures[" + iStr + "]", m_Data.texIndex);

        // m_Data.texIndex++;
    }
    auto &batch = m_Data.batchMap[texture->getId()];
    batch.quadPtr[m_Data.indexCount].color = glm::vec4(alpha);
    batch.quadPtr[m_Data.indexCount].model = model;
    batch.quadPtr[m_Data.indexCount].textureModel = textureModel;
    batch.quadPtr[m_Data.indexCount].material = material;

    batch.indexCount++;
}

void OpenGLRender::addPointLight(const PointLight &light,
                                 const glm::vec2 &position) {
    auto iStr = std::to_string(m_Data.pointLightIndex);

    m_Data.shader->setFloat2("pointLights[" + iStr + "].position", position.x,
                             position.y);
    m_Data.shader->setFloat3("pointLights[" + iStr + "].ambient",
                             light.ambient.x, light.ambient.y, light.ambient.z);
    m_Data.shader->setFloat3("pointLights[" + iStr + "].diffuse",
                             light.diffuse.x, light.diffuse.y, light.diffuse.z);
    m_Data.shader->setFloat3("pointLights[" + iStr + "].specular",
                             light.specular.x, light.specular.y,
                             light.specular.z);
    m_Data.shader->setFloat("pointLights[" + iStr + "].constant",
                            light.constant);
    m_Data.shader->setFloat("pointLights[" + iStr + "].linear", light.linear);
    m_Data.shader->setFloat("pointLights[" + iStr + "].quadratic",
                            light.quadratic);

    m_Data.pointLightIndex++;
}

void OpenGLRender::addSpotLight(const SpotLight &light,
                                const glm::vec2 &position) {
    auto iStr = std::to_string(m_Data.spotLightsIndex);

    m_Data.shader->setFloat2("spotLights[" + iStr + "].position", position.x,
                             position.y);
    m_Data.shader->setFloat2("spotLights[" + iStr + "].direction",
                             light.direction.x, light.direction.y);
    m_Data.shader->setFloat("spotLights[" + iStr + "].cutOff", light.cutOff);
    m_Data.shader->setFloat("spotLights[" + iStr + "].outerCutOff",
                            light.outerCutOff);
    m_Data.shader->setFloat3("spotLights[" + iStr + "].ambient",
                             light.ambient.x, light.ambient.y, light.ambient.z);
    m_Data.shader->setFloat3("spotLights[" + iStr + "].diffuse",
                             light.diffuse.x, light.diffuse.y, light.diffuse.z);
    m_Data.shader->setFloat3("spotLights[" + iStr + "].specular",
                             light.specular.x, light.specular.y,
                             light.specular.z);
    m_Data.shader->setFloat("spotLights[" + iStr + "].constant",
                            light.constant);
    m_Data.shader->setFloat("spotLights[" + iStr + "].linear", light.linear);
    m_Data.shader->setFloat("spotLights[" + iStr + "].quadratic",
                            light.quadratic);

    m_Data.spotLightsIndex++;
}

void OpenGLRender::drawLines(std::shared_ptr<Shader> shader,
                             std::shared_ptr<VertexArray> vertexArray) {
    shader->bind();
    vertexArray->bind();
    glDrawElements(GL_LINES, vertexArray->getIndexBuffer()->getCount(),
                   GL_UNSIGNED_INT, nullptr);
    GL_CHECK();
}

void OpenGLRender::drawTriangles(std::shared_ptr<Shader> shader,
                                 std::shared_ptr<VertexArray> vertexArray) {
    shader->bind();
    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(),
                   GL_UNSIGNED_INT, nullptr);
    GL_CHECK();
}

void OpenGLRender::drawTexture(std::shared_ptr<Shader> shader,
                               std::shared_ptr<VertexArray> vertexArray,
                               std::shared_ptr<Texture> texture) {
    shader->bind();
    texture->bind();
    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(),
                   GL_UNSIGNED_INT, nullptr);

    texture->unbind();
}

void OpenGLRender::drawTexture(std::shared_ptr<Shader> shader,
                               std::shared_ptr<VertexArray> vertexArray,
                               Texture *texture) {
    shader->bind();
    texture->bind();
    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(),
                   GL_UNSIGNED_INT, nullptr);

    texture->unbind();
}

} // namespace Engine
