#pragma once

#include "ActionManager.hpp"

#include <cmath>

class GrandTransformation : public Action {
  private:
    std::shared_ptr<Engine::Entity> m_Grand;
    std::shared_ptr<Engine::Entity> m_GrandWolf;

  public:
    GrandTransformation(int x) : Action(x, 0) {}

    virtual void init(Context *context) override {
        m_Grand = context->getEntity("grand");
        m_GrandWolf = context->getEntity("grand-wolf");
    }

    virtual void exec() override {
        m_Executed = true;
        m_Grand->getComponent<Engine::AnimationComponent>()->isActive = true;
        m_GrandWolf->getComponent<Engine::AnimationComponent>()->isActive =
            true;
    }
};
