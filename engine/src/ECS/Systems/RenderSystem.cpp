#include "RenderSystem.hpp"
#include "Application.hpp"
#include "DirectionComponent.hpp"
#include "FrameAnimationComponent.hpp"
#include "FrameComponent.hpp"
#include "IgnoreLightComponent.hpp"
#include "LocationComponent.hpp"
#include "MaterialComponent.hpp"
#include "ParalaxScrollingComponent.hpp"
#include "ParentComponent.hpp"
#include "RenderComponent.hpp"
#include "TextureComponent.hpp"
#include "cmath"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

void RenderSystem::exec(EntityManager &entities) {
    auto &render = Application::get().getRender();
    auto &window = Application::get().getWindow();
    auto &camera = Application::get().getCamera();
    auto &shaders = Application::get().getShaders();
    auto &textures = Application::get().getTextures();

    float windowWidth = static_cast<float>(window.getWidth());
    float windowHeight = static_cast<float>(window.getHeight());

    for (auto entity : entities.getAll()) {
        if (entity->hasComponent<RenderComponent>()) {
            auto c_render = entity->getComponent<RenderComponent>();

            if (!c_render->isActive) {
                continue;
            }

            auto location = entity->getComponent<LocationComponent>();
            auto material = entity->getComponent<MaterialComponent>();

            float paralaxScale = 1.0;

            if (entity->hasComponent<ParalaxScrollingComponent>()) {
                auto pralax = entity->getComponent<ParalaxScrollingComponent>();
                paralaxScale = pralax->scale;
            }

            float dx = location->x - camera.x * paralaxScale;
            float dy = location->y - camera.y * paralaxScale;

            float x = dx / windowWidth * 2.0 - 1;
            float y = dy / windowHeight * 2.0 - 1;

            float scaleX = c_render->width / windowWidth;
            float scaleY = c_render->height / windowHeight;

            glm::mat4 model(1.0);
            model = glm::translate(model, glm::vec3(x, y, 0));
            model = glm::rotate(model, location->angle,
                                glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0));

            std::shared_ptr<Shader> shader;

            if (entity->hasComponent<IgnoreLightComponent>()) {
                shader = shaders.get("plain");
            } else {
                shader = shaders.get("plain-with-light");

                shader->setFloat3("material.ambient", material->ambient.x,
                                  material->ambient.y, material->ambient.z);
                shader->setFloat3("material.diffuse", material->diffuse.x,
                                  material->diffuse.y, material->diffuse.z);
                shader->setFloat3("material.specular", material->specular.x,
                                  material->specular.y, material->specular.z);
                shader->setFloat("material.shininess", material->shininess);
            }

            shader->setMatrix4("model", glm::value_ptr(model));
            render.drawTriangles(shader, c_render->vertexArray);
        }

        if (entity->hasComponent<TextureComponent>()) {
            auto texture = entity->getComponent<TextureComponent>();

            if (!texture->isActive) {
                continue;
            }

            auto material = entity->getComponent<MaterialComponent>();
            auto location = entity->getComponent<LocationComponent>();
            auto direction = entity->getComponent<DirectionComponent>();

            float paralaxScale = 1.0;
            float parentX = 0;
            float parentY = 0;

            if (entity->hasComponent<ParalaxScrollingComponent>()) {
                auto pralax = entity->getComponent<ParalaxScrollingComponent>();
                paralaxScale = pralax->scale;
            }

            if (entity->hasComponent<ParentComponent>()) {
                std::string parentId =
                    entity->getComponent<ParentComponent>()->parent;
                auto parent = entities.get(parentId);
                auto parentLocation = parent->getComponent<LocationComponent>();

                parentX = parentLocation->x;
                parentY = parentLocation->y;
            }

            float x = (location->x + parentX - camera.x * paralaxScale) /
                          windowWidth * 2.0 -
                      1;
            float y = (location->y + parentY - camera.y * paralaxScale) /
                          windowHeight * 2.0 -
                      1;

            float scaleX = location->scale * texture->width / windowWidth;
            float scaleY = location->scale * texture->height / windowHeight;

            glm::mat4 model(1.0);
            model = glm::translate(model, glm::vec3(x, y, 0.0f));
            model = glm::rotate(model, location->angle,
                                glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0));

            glm::mat4 textureModel(1.0);

            if (entity->hasComponent<FrameAnimationComponent>()) {
                auto animation =
                    entity->getComponent<FrameAnimationComponent>();
                auto current = animation->getCurrent();

                float x = current.dx * animation->frameIndex;
                if (direction->x > 0) {
                    x = x + current.dx;
                }

                float y = current.y;

                textureModel =
                    glm::translate(textureModel, glm::vec3(x, y, 0.0f));
            }

            if (entity->hasComponent<FrameComponent>()) {
                auto frame = entity->getComponent<FrameComponent>();

                float x = texture->source.w * frame->index;

                textureModel =
                    glm::translate(textureModel, glm::vec3(x, 0.0f, 0.0f));
            }

            if (direction->x > 0) {
                textureModel = glm::rotate(textureModel, 3.14f,
                                           glm::vec3(0.0f, 1.0f, 0.0f));
            }

            std::shared_ptr<Shader> shader;

            if (entity->hasComponent<IgnoreLightComponent>()) {
                shader = shaders.get("texture");
            } else {
                shader = shaders.get("texture-with-light");

                shader->setFloat3("material.ambient", material->ambient.x,
                                  material->ambient.y, material->ambient.z);
                shader->setFloat3("material.diffuse", material->diffuse.x,
                                  material->diffuse.y, material->diffuse.z);
                shader->setFloat3("material.specular", material->specular.x,
                                  material->specular.y, material->specular.z);
                shader->setFloat("material.shininess", material->shininess);
            }

            shader->setMatrix4("model", glm::value_ptr(model));
            shader->setMatrix4("texture_model", glm::value_ptr(textureModel));
            shader->setFloat("alpha", texture->alpha);

            render.drawTexture(shader, texture->vertexArray,
                               textures.get(texture->name));
        }
    }
}
} // namespace Engine
