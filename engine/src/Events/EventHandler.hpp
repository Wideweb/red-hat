
#pragma once

#include "EventDispatcher.hpp"
#include <unordered_map>
#include <memory>

namespace Engine {

using EventTypeId = const std::type_info *;

class EventHandler {
  private:
    std::unordered_map<EventTypeId, std::shared_ptr<IEventDispatcher>> m_dispatchers;

  public:
    template <typename TEvent> void add(EventDelegate<TEvent> delegate) {
        auto dispatcher = std::static_pointer_cast<EventDispatcher<TEvent>>(
            m_dispatchers[&typeid(TEvent)]);

        if (dispatcher == nullptr) {
            dispatcher.reset(new EventDispatcher<TEvent>());
            m_dispatchers[&typeid(TEvent)] = dispatcher;
        }

        dispatcher->addEventCallback(delegate);
    }

    template <typename TEvent> void send(TEvent &event) {
        auto dispatcher = std::static_pointer_cast<EventDispatcher<TEvent>>(
            m_dispatchers[&typeid(TEvent)]);

        if (dispatcher != nullptr) {
            dispatcher->dispatch(event);
        }
    }
};

} // namespace Engine
