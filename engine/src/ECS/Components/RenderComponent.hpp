#pragma once

#include "Buffer.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

#include <glm/vec4.hpp>

namespace Engine {

class RenderComponent : public Component {
  public:
    int width;
    int height;
    QuadVertices vertices;
    glm::vec4 color;

    RenderComponent(int width, int height, glm::vec4 color);

    virtual void serialize(std::ostringstream &out) override {
        out << "render ";
        out << width << " ";
        out << height << " ";
        out << color.x << " ";
        out << color.y << " ";
        out << color.z;
        out << std::endl;
    }

  private:
    void init(std::vector<float> &vertices, std::vector<uint32_t> &indexes,
              int width, int height);
};

} // namespace Engine