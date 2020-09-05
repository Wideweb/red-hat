#pragma once

#include "Entity.hpp"
#include "Math.hpp"
#include "Render.hpp"

namespace Engine {

class MaterialComponent : public Component {
  public:
    Material material;

    MaterialComponent() {
        material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
        material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        material.shininess = 32.0f;
    }

    virtual void serialize(std::ostringstream &out) override {
        out << "material ";
        out << material.ambient.x << " ";
        out << material.diffuse.x << " ";
        out << material.specular.x << " ";
        out << material.shininess;
        out << std::endl;
    }
};

} // namespace Engine