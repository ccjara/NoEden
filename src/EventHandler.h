#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

class EventHandler
{
private:
    EventCallback callback;
    uint32_t eventType;
    bool attached;
public:
    EventHandler(uint32_t eventType, const EventCallback& callback);
    ~EventHandler();

    void detach() noexcept;
    bool isDetached() const noexcept;
    void trigger(const SDL_Event& e);
    uint32_t getEventType() const noexcept;
};

#endif
