#include "TextureComponent.hpp"

namespace Engine {

TextureComponent::TextureComponent(const std::string &name, const Rect &source,
                                   int width, int height, float alpha)
    : name(name), source(source), width(width), height(height), alpha(alpha) {

    float x0 = source.x;
    float y0 = source.y;
    float x1 = x0 + source.w;
    float y1 = source.y + source.h;

    vertices[0].position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertices[1].position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vertices[2].position = glm::vec3(1.0f, -1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 1.0f, 0.0f);

    vertices[0].texCoord = glm::vec2(x0, y1);
    vertices[1].texCoord = glm::vec2(x0, y0);
    vertices[2].texCoord = glm::vec2(x1, y1);
    vertices[3].texCoord = glm::vec2(x1, y0);
}

} // namespace Engine