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

            auto c_location = entity->getComponent<LocationComponent>();
            auto c_material = entity->getComponent<MaterialComponent>();

            float paralaxScale = 1.0;

            if (entity->hasComponent<ParalaxScrollingComponent>()) {
                auto pralax = entity->getComponent<ParalaxScrollingComponent>();
                paralaxScale = pralax->scale;
            }

            float dx = c_location->x - camera.x * paralaxScale;
            float dy = c_location->y - camera.y * paralaxScale;

            float x = dx / windowWidth * 2.0 - 1;
            float y = dy / windowHeight * 2.0 - 1;

            float scaleX = c_render->width / windowWidth;
            float scaleY = c_render->height / windowHeight;

            Mat2x3 scale = Mat2x3::scale(scaleX, scaleY);
            Mat2x3 rotate = Mat2x3::rotate(c_location->angle);
            Mat2x3 move = Mat2x3::move(Vec2(x, y));
            Mat2x3 model = move * scale * rotate;

            std::shared_ptr<Shader> shader;

            if (entity->hasComponent<IgnoreLightComponent>()) {
                shader = shaders.get("plain");
            } else {
                shader = shaders.get("plain-with-light");

                shader->setFloat3("material.ambient", c_material->ambient.x,
                                  c_material->ambient.y, c_material->ambient.z);
                shader->setFloat3("material.diffuse", c_material->diffuse.x,
                                  c_material->diffuse.y, c_material->diffuse.z);
                shader->setFloat3("material.specular", c_material->specular.x,
                                  c_material->specular.y,
                                  c_material->specular.z);
                shader->setFloat("material.shininess", c_material->shininess);
            }

            shader->setMatrix2x3("model", model.data());
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

            Mat2x3 scale = Mat2x3::scale(scaleX, scaleY);
            Mat2x3 rotate = Mat2x3::rotate(location->angle);
            Mat2x3 move = Mat2x3::move(Vec2(x, y));
            Mat2x3 model = move * scale * rotate;

            Mat2x3 textureModel = Mat2x3::identity();
            if (direction->x > 0) {
                textureModel = Mat2x3::flipY();
            }

            if (entity->hasComponent<FrameAnimationComponent>()) {
                auto animation =
                    entity->getComponent<FrameAnimationComponent>();
                auto current = animation->getCurrent();

                float x = current.dx * animation->frameIndex;
                if (direction->x > 0) {
                    x = x + current.dx;
                }

                float y = current.y;

                textureModel = Mat2x3::move(Vec2(x, y)) * textureModel;
            }

            if (entity->hasComponent<FrameComponent>()) {
                auto frame = entity->getComponent<FrameComponent>();

                float x = texture->source.w * frame->index;

                textureModel = Mat2x3::move(Vec2(x, 1.0)) * textureModel;
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

            shader->setMatrix2x3("model", model.data());
            shader->setMatrix2x3("texture_model", textureModel.data());
            shader->setFloat("alpha", texture->alpha);

            render.drawTexture(shader, texture->vertexArray,
                               textures.get(texture->name));
        }
    }
}
} // namespace Engine
