#include "CollisionSystem.hpp"
#include "Application.hpp"
#include "CollisionComponent.hpp"
#include "LocationComponent.hpp"
#include "ObstacleComponent.hpp"
#include "VelocityComponent.hpp"

#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

namespace Engine {

void CollisionSystem::exec(EntityManager &entities) {
    auto &app = Application::get();
    auto &eventHandler = app.getEventHandler();

    std::vector<CollisionShape> colliders;
    for (auto &entity : entities.getAll()) {
        if (!entity->hasComponent<CollisionComponent>()) {
            continue;
        }

        auto collision = entity->getComponent<CollisionComponent>();
        auto location = entity->getComponent<LocationComponent>();

        collision->entity = nullptr;

        glm::vec2 move(location->x, location->y);

        if (entity->hasComponent<VelocityComponent>()) {
            auto velocity = entity->getComponent<VelocityComponent>();
            move = move + glm::vec2(velocity->x, velocity->y);
        }

        std::vector<glm::vec2> vertices;
        std::transform(collision->vertices.begin(), collision->vertices.end(),
                       std::back_inserter(vertices),
                       [&](glm::vec2 &v) { return v + move; });

        colliders.emplace_back(entity->getName(), vertices,
                               entity->hasComponent<ObstacleComponent>());
    }

    if (colliders.empty()) {
        return;
    }

    std::vector<CollisionResult> results =
        m_CollisionDetection.detect(colliders);

    for (auto &result : results) {
        auto entity = entities.get(result.shape1);
        auto location = entity->getComponent<LocationComponent>();
        auto velocity = entity->getComponent<VelocityComponent>();
        auto collision = entity->getComponent<CollisionComponent>();

        glm::vec2 gravity(0, -1);
        float cos = glm::dot(gravity, glm::normalize(result.mtv));

        if (cos < -0.7) {
            location->y += velocity->y;
            location->y += result.mtv.y;
            velocity->y = 0;
        } else {
            location->x += velocity->x;
            location->x += result.mtv.x;
            velocity->x = 0;
        }

        collision->entity = entities.get(result.shape2);

        BeginCollisionEvent event;
        event.first = result.shape1;
        event.second = result.shape2;
        eventHandler.send<BeginCollisionEvent>(event);
    }
}

} // namespace Engine
