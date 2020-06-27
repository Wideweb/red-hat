#pragma once

#include "AnimationComponent.hpp"
#include "Task.hpp"

#include <cmath>

namespace Engine {

class RunAnimationTask : public Task {
  public:
    RunAnimationTask() {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        auto entity = blackboard->getPtr<Entity>("entity");
        entity->getComponent<AnimationComponent>()->isActive = true;

        return TaskStatus::Success;
    }
};

} // namespace Engine
