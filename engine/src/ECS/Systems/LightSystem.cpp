#include "LightSystem.hpp"
#include "Application.hpp"
#include "FrameAnimationComponent.hpp"
#include "LocationComponent.hpp"
#include "MaterialComponent.hpp"
#include "ParalaxScrollingComponent.hpp"
#include "PointLightComponent.hpp"
#include "TextureComponent.hpp"

#include "cmath"
#include <glm/vec2.hpp>
#include <vector>

namespace Engine {

void LightSystem::exec(EntityManager &entities) {
    auto &render = Application::get().getRender();
    auto &window = Application::get().getWindow();
    auto &camera = Application::get().getCamera();

    float windowWidth = static_cast<float>(window.getWidth());
    float windowHeight = static_cast<float>(window.getHeight());

    auto pointLights = entities.getByComponent<PointLightComponent>();
    if (pointLights.size() == 0) {
        return;
    }

    for (size_t i = 0; i < pointLights.size(); i++) {
        auto entity = pointLights[i];
        auto light = entity->getComponent<PointLightComponent>()->light;
        auto location = entity->getComponent<LocationComponent>();

        float paralaxScale = 1.0;

        if (entity->hasComponent<ParalaxScrollingComponent>()) {
            auto pralax = entity->getComponent<ParalaxScrollingComponent>();
            paralaxScale = pralax->scale;
        }

        float x =
            (location->x - camera.x * paralaxScale) / windowWidth * 2.0 - 1;
        float y =
            (location->y - camera.y * paralaxScale) / windowHeight * 2.0 - 1;

        render.addPointLight(light, glm::vec2(x, y));
    }
}

} // namespace Engine
