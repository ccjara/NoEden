#include "stdafx.h"

EventManager::EventManager()
{
}

void EventManager::attach(std::shared_ptr<EventHandler> handler)
{
    auto&& targetHandlers = handlers[handler->getEventType()];

    targetHandlers.push_back(handler);

    LOG(DEBUG)
        << "Added handler for event type "
        << handler->getEventType()
        << " - "
        << targetHandlers.size()
        << " handler(s) registered so far.";
}

void EventManager::trigger(const SDL_Event& e)
{
    auto&& targetHandlers = handlers[e.type];

    for (auto iter = targetHandlers.begin(); iter != targetHandlers.end(); iter++) {
        auto h = iter->lock();
        if (h == nullptr || h->isDetached()) {
            LOG(DEBUG) << "Found one bad ref or detached listener @" << h << " - erasing and skipping trigger.";
            targetHandlers.erase(iter);
            continue;
        }
        h->trigger(e);
    }
}