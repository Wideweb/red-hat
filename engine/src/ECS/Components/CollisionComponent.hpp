#pragma once

#include "Entity.hpp"
#include <glm/vec2.hpp>
#include <memory>
#include <string>

namespace Engine {

struct BeginCollisionEvent {
    std::string first;
    std::string second;
};

struct EndCollisionEvent {
    std::string first;
    std::string second;
};

class CollisionComponent : public Component {
  public:
    std::vector<glm::vec2> vertices;
    std::shared_ptr<Entity> entity;

    CollisionComponent(float width, float height) {
        float wHalf = width / 2;
        float hHalf = height / 2;

        vertices.emplace_back(-wHalf, -hHalf);
        vertices.emplace_back(-wHalf, hHalf);
        vertices.emplace_back(wHalf, hHalf);
        vertices.emplace_back(wHalf, -hHalf);
    }

    CollisionComponent(const std::vector<glm::vec2> &vertices)
        : vertices(vertices) {}

    virtual void serialize(std::ostringstream &out) override {
        float width = vertices[2].x * 2;
        float height = vertices[2].y * 2;

        out << "collisionRect"
            << " ";
        out << width << " ";
        out << height;
        out << std::endl;
    }
};

} // namespace Engine