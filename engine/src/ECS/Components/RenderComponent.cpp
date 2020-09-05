#include "RenderComponent.hpp"

namespace Engine {

RenderComponent::RenderComponent(int width, int height, glm::vec4 color)
    : width(width), height(height), color(color) {
    vertices[0].position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertices[1].position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vertices[2].position = glm::vec3(1.0f, -1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 1.0f, 0.0f);
}

} // namespace Engine