#pragma once

#include "Shader.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

class ShaderManager {
  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Map;

  public:
    void load(const std::string &name, const std::string &vertexPath,
              const std::string &fragmentPath);
    std::shared_ptr<Shader> get(const std::string &name);
};

} // namespace Engine
