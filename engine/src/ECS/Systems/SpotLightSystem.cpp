#include "SpotLightSystem.hpp"
#include "Application.hpp"
#include "DirectionComponent.hpp"
#include "FrameAnimationComponent.hpp"
#include "LocationComponent.hpp"
#include "MaterialComponent.hpp"
#include "ParalaxScrollingComponent.hpp"
#include "ParentComponent.hpp"
#include "SpotLightComponent.hpp"

#include "cmath"
#include <glm/vec2.hpp>
#include <vector>

namespace Engine {

void SpotLightSystem::exec(EntityManager &entities) {
    auto &render = Application::get().getRender();
    auto &window = Application::get().getWindow();
    auto &camera = Application::get().getCamera();

    float windowWidth = static_cast<float>(window.getWidth());
    float windowHeight = static_cast<float>(window.getHeight());

    auto lights = entities.getByComponent<SpotLightComponent>();
    if (lights.size() == 0) {
        return;
    }

    for (size_t i = 0; i < lights.size(); i++) {
        auto entity = lights[i];
        auto light = entity->getComponent<SpotLightComponent>()->light;
        auto location = entity->getComponent<LocationComponent>();

        float parentX = 0;
        float parentY = 0;
        int flip = 1;

        if (entity->hasComponent<ParentComponent>()) {
            std::string parentId =
                entity->getComponent<ParentComponent>()->parent;
            auto parent = entities.get(parentId);
            auto parentLocation = parent->getComponent<LocationComponent>();

            parentX = parentLocation->x;
            parentY = parentLocation->y;

            auto direction = parent->getComponent<DirectionComponent>();
            flip = direction->x;
        }

        float x = (location->x + parentX - camera.x) / windowWidth * 2.0 - 1;
        float y = (location->y + parentY - camera.y) / windowHeight * 2.0 - 1;

        render.addSpotLight(light, glm::vec2(x, y));
    }
}

} // namespace Engine
