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

    m_Data.quadBuffer.reset(VertexBuffer::create(sizeof(Quad)));
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

    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::create({0, 1, 2, 1, 2, 3}));
    m_Data.vertexArray->setIndexBuffer(indexBuffer);

    std::string vertexSrc = File::read("./assets/shaders/vertex-shader.glsl");
    std::string fragmentSrc =
        File::read("./assets/shaders/fragment-shader.glsl");

    m_Data.shader.reset(Shader::create(vertexSrc, fragmentSrc));
}

void OpenGLRender::beginScene() {
    m_Data.pointLightIndex = 0;
    m_Data.spotLightsIndex = 0;
    m_Data.index = 0;
}

void OpenGLRender::endScene() { std::cout << m_Data.index << std::endl; }

void OpenGLRender::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    GL_CHECK();
}

void OpenGLRender::setViewport(uint16_t width, uint16_t height) {
    m_Width = width;
    m_Height = height;
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
    Quad quad;
    quad.color = color;
    quad.model = model;
    quad.texIndex = -1;
    quad.material = material;

    m_Data.vertexBuffer->setData(vertices.data(), sizeof(QuadVertices));
    m_Data.quadBuffer->setData(&quad, sizeof(Quad));

    this->drawTriangles(m_Data.shader, m_Data.vertexArray);

    m_Data.index++;
}

void OpenGLRender::drawTexture(const QuadVertices &vertices,
                               const glm::mat4 &model,
                               const glm::mat4 &textureModel,
                               const Material &material,
                               const std::shared_ptr<Texture> &texture,
                               float alpha) {
    Quad quad;
    quad.color = glm::vec4(alpha);
    quad.model = model;
    quad.textureModel = textureModel;
    quad.texIndex = 0;
    quad.material = material;

    m_Data.vertexBuffer->setData(vertices.data(), sizeof(QuadVertices));
    m_Data.quadBuffer->setData(&quad, sizeof(Quad));

    this->drawTexture(m_Data.shader, m_Data.vertexArray, texture);

    m_Data.index++;
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

    m_Data.shader->setInt("pointLightsNumber", m_Data.pointLightIndex);
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

    m_Data.shader->setInt("spotLightsNumber", m_Data.spotLightsIndex);
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
