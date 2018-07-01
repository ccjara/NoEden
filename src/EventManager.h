#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

class EventManager
{
private:
    std::map<uint32_t, std::vector<std::weak_ptr<EventHandler>>> handlers;
public:
    EventManager();

    void attach(std::shared_ptr<EventHandler> handler);
    void trigger(const SDL_Event& e);
};

#endif
