#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

using EventCallback = std::function<void(const SDL_Event& e)>;

class EventManager
{
private:
    using Identifier = uint32_t;

    Identifier nextId;

    std::map<uint32_t, std::map<Identifier, EventCallback>> listeners;
public:
    EventManager() : nextId(0) {}

    Identifier subscribe(uint32_t eventId, EventCallback& callback)
    {
        auto id = nextId;
        listeners[eventId][id] = callback;
        nextId++;
        return id;
    }

    void unsubscribe(uint32_t eventId, Identifier id)
    {
        listeners[eventId].erase(id);
    }

    void trigger(const SDL_Event& e)
    {
        for (const auto& l : listeners[e.type]) {
            l.second(e);
        }
    }
};

#endif
