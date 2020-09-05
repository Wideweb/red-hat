#include "EntitySerializer.hpp"
#include "AnimationLoader.hpp"

#include <glm/vec3.hpp>

namespace Engine {

Entity *EntitySerializer::from(std::istringstream &in) {
    std::string attribute;
    Entity *entity;

    while (!in.eof() && in >> attribute) {
        if (attribute == "entity") {
            std::string id;
            in >> id;
            entity = new Entity(id);
        }

        if (attribute == "location") {
            float x, y;
            in >> x >> y;
            entity->addComponent<LocationComponent>(x, y);
        }

        if (attribute == "velocity") {
            float x, y;
            in >> x >> y;
            entity->addComponent<VelocityComponent>(x, y);
        }

        if (attribute == "collisionRect") {
            float w, h;
            in >> w >> h;
            entity->addComponent<CollisionComponent>(w, h);
        }

        if (attribute == "groundCollision") {
            float w, h;
            in >> w >> h;
            entity->addComponent<GroundCollisionComponent>(w, h);
        }

        if (attribute == "colliderTriangle") {
            glm::vec2 v0, v1, v2;
            in >> v0.x >> v0.y;
            in >> v1.x >> v1.y;
            in >> v2.x >> v2.y;

            std::vector<glm::vec2> vertices = {v0, v1, v2};

            entity->addComponent<CollisionComponent>(vertices);
        }

        if (attribute == "obstacle") {
            entity->addComponent<ObstacleComponent>();
        }

        if (attribute == "ground") {
            entity->addComponent<GroundComponent>();
        }

        if (attribute == "render") {
            int w, h;
            glm::vec4 color;

            in >> w >> h;
            in >> color.x >> color.y >> color.z;
            color.a = 1.0f;

            entity->addComponent<RenderComponent>(w, h, color);
            entity->addComponent<MaterialComponent>();
        }

        if (attribute == "texture") {
            std::string id;
            Rect source;
            float w, h;
            float alpha;

            in >> id;
            in >> source.x >> source.y >> source.w >> source.h;
            in >> w >> h;
            in >> alpha;

            entity->addComponent<TextureComponent>(id, source, w, h, alpha);
            entity->addComponent<MaterialComponent>();
        }

        if (attribute == "material") {
            float r, g, b, shininess;

            in >> r >> g >> b >> shininess;

            auto material = entity->getComponent<MaterialComponent>();
            material->material = {.ambient = glm::vec3(r, g, b),
                                  .diffuse = glm::vec3(r, g, b),
                                  .specular = glm::vec3(r, g, b),
                                  .shininess = shininess};
        }

        if (attribute == "pointLight") {
            PointLight light;

            in >> light.ambient.x >> light.ambient.y >> light.ambient.z;
            in >> light.diffuse.x >> light.diffuse.y >> light.diffuse.z;
            in >> light.specular.x >> light.specular.y >> light.specular.z;
            in >> light.constant >> light.linear >> light.quadratic;

            entity->addComponent<PointLightComponent>(light);
        }

        if (attribute == "spotLight") {
            SpotLight light;

            in >> light.ambient.x >> light.ambient.y >> light.ambient.z;
            in >> light.diffuse.x >> light.diffuse.y >> light.diffuse.z;
            in >> light.specular.x >> light.specular.y >> light.specular.z;
            in >> light.constant >> light.linear >> light.quadratic;
            in >> light.direction.x >> light.direction.y;
            in >> light.cutOff >> light.outerCutOff;

            entity->addComponent<SpotLightComponent>(light);
        }

        if (attribute == "ignoreLight") {
            entity->addComponent<IgnoreLightComponent>();
        }

        if (attribute == "animation") {
            std::string path;
            bool isActive;

            in >> path;
            in >> isActive;

            auto scene = AnimationLoader::load(path);

            entity->addComponent<AnimationComponent>(scene)->isActive =
                isActive;
        }

        if (attribute == "frameAnimation") {
            float y;
            float dx;
            int framesNumber;
            float time;

            in >> y >> dx >> framesNumber >> time;

            auto wait = FrameAnimation(y, dx, framesNumber, time);

            in >> y >> dx >> framesNumber >> time;

            auto move = FrameAnimation(y, dx, framesNumber, time);

            entity->addComponent<FrameAnimationComponent>(wait, move);
        }

        if (attribute == "frame") {
            int framesNumber;
            entity->addComponent<FrameComponent>(framesNumber);
        }

        if (attribute == "paralaxScrolling") {
            float scale;
            in >> scale;
            entity->addComponent<ParalaxScrollingComponent>(scale);
        }

        if (attribute == "camera") {
            int leftBound, rightBound, offset;
            in >> leftBound >> rightBound >> offset;
            entity->addComponent<CameraComponent>(leftBound, rightBound,
                                                  offset);
        }

        if (attribute == "rigitBody") {
            float weight;
            in >> weight;
            entity->addComponent<RigitBodyComponent>(weight);
        }

        if (attribute == "parent") {
            std::string parent;
            in >> parent;
            entity->addComponent<ParentComponent>(parent);
        }

        if (attribute == "steering") {
            std::string target;
            in >> target;
            entity->addComponent<SteeringComponent>(target);
        }

        if (attribute == "enemy") {
            entity->addComponent<EnemyComponent>();
        }

        if (attribute == "attack") {
            Rect r;
            in >> r.x >> r.y >> r.w >> r.h;
            entity->addComponent<AttackComponent>(r);
        }

        if (attribute == "direction") {
            int x, y;
            in >> x >> y;
            entity->addComponent<DirectionComponent>(x, y);
        }

        if (attribute == "emitter") {
            std::string prefab;
            int buffer, width;
            float interval;

            in >> prefab;
            in >> buffer >> width;
            in >> interval;

            entity->addComponent<EmitterComponent>(prefab, buffer, width,
                                                   interval);
        }

        if (attribute == "inactive") {
            entity->isActive = false;
        }

        if (attribute == "group") {
            std::string group;
            in >> group;
            entity->addComponent<GroupComponent>(group);
        }

        in >> std::ws;
    }

    return entity;
}

} // namespace Engine
