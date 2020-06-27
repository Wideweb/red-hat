#pragma once

#include "ActionManager.hpp"
#include "ScreenManager.hpp"

class EndGameAction : public Action {
  public:
    EndGameAction(int x, float delay) : Action(x, delay) {}

    virtual void init(Context *context) override {}

    virtual void exec() override {
        m_Executed = true;
        ScreenManager::get().goTo("title");
    }
};
