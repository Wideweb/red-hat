#pragma once

#include "Entity.hpp"
#include "Render.hpp"

namespace Engine {

class SpotLightComponent : public Component {
  public:
    SpotLight light;

    SpotLightComponent(const SpotLight &light) : light(light) {}

    virtual void serialize(std::ostringstream &out) override {
        out << "spotLight ";
        out << light.ambient.x << " " << light.ambient.y << " "
            << light.ambient.z << " ";
        out << light.diffuse.x << " " << light.diffuse.y << " "
            << light.diffuse.z << " ";
        out << light.specular.x << " " << light.specular.y << " "
            << light.specular.z << " ";
        out << light.constant << " ";
        out << light.linear << " ";
        out << light.quadratic << " ";
        out << light.cutOff << " ";
        out << light.outerCutOff << " ";
        out << std::endl;
    }
};

} // namespace Engine