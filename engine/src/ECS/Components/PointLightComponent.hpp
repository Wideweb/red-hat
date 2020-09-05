#pragma once

#include "Entity.hpp"
#include "Render.hpp"

namespace Engine {

class PointLightComponent : public Component {
  public:
    PointLight light;

    PointLightComponent(const PointLight &light) : light(light) {}

    virtual void serialize(std::ostringstream &out) override {
        out << "pointLight ";
        out << light.ambient.x << " " << light.ambient.y << " "
            << light.ambient.z << " ";
        out << light.diffuse.x << " " << light.diffuse.y << " "
            << light.diffuse.z << " ";
        out << light.specular.x << " " << light.specular.y << " "
            << light.specular.z << " ";
        out << light.constant << " ";
        out << light.linear << " ";
        out << light.quadratic << std::endl;
    }
};

} // namespace Engine