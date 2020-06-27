#pragma once

#include "AnimationComponent.hpp"
#include "Task.hpp"

#include <cmath>

namespace Engine {

class StopAnimationTask : public Task {
  public:
    StopAnimationTask() {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        auto entity = blackboard->getPtr<Entity>("entity");
        entity->getComponent<AnimationComponent>()->isActive = false;

        return TaskStatus::Success;
    }
};

} // namespace Engine
