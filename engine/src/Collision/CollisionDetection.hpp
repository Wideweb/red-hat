#pragma once

#include "BroadPhaseAlgorithm.hpp"
#include "CollisionShape.hpp"
#include "NarrowPhaseAlgorithm.hpp"

#include <glm/vec2.hpp>

namespace Engine {

struct CollisionResult {
    std::string shape1;
    std::string shape2;
    glm::vec2 mtv;

    CollisionResult() {}
    CollisionResult(std::string shape1, std::string shape2, glm::vec2 mtv)
        : shape1(shape1), shape2(shape2), mtv(mtv) {}
};

class CollisionDetection {
  private:
    BroadPhaseAlgorithm m_BroadPhase;
    NarrowPhaseAlgorithm m_NarrowPhase;

  public:
    std::vector<CollisionResult>
    detect(const std::vector<CollisionShape> &shapesd);
};

} // namespace Engine