#pragma once

#include "Buffer.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

#include <string>
#include <vector>

namespace Engine {

class TextureComponent : public Component {
  public:
    int width;
    int height;
    float alpha;

    QuadVertices vertices;

    std::string name;
    Rect source;

    TextureComponent(const std::string &name, const Rect &source, int width,
                     int height, float alpha);

    virtual void serialize(std::ostringstream &out) override {
        out << "texture ";
        out << name << " ";
        out << source << " ";
        out << width << " ";
        out << height << " ";
        out << std::endl;
    }
};

} // namespace Engine