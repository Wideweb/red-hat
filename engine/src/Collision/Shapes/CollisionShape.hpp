#pragma once

#include <glm/vec2.hpp>
#include <string>
#include <vector>

namespace Engine {

struct CollisionShape {
    std::string name;
    std::vector<glm::vec2> vertices;
    bool isStatic;

    CollisionShape() {}
    CollisionShape(const std::string &name,
                   const std::vector<glm::vec2> &vertices, bool isStatic)
        : name(name), vertices(vertices), isStatic(isStatic) {}
};

} // namespace Engine