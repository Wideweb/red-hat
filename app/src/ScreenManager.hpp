#pragma once

#include "Screen.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class ScreenManager {
  private:
    std::shared_ptr<Screen> m_Current;
    std::shared_ptr<Screen> m_New;
    std::unordered_map<std::string, std::shared_ptr<Screen>> m_ScreenMap;
    Context *m_Context;
    static ScreenManager *s_Instance;
    bool m_IsTransitioning = false;

  public:
    ScreenManager();
    ~ScreenManager();

    void init(Context *context);
    void update();
    void add(const std::string &screenId, Screen *screen);
    void goTo(const std::string &screenId);
    void unload();

    static ScreenManager &get() { return *s_Instance; }
};