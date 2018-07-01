#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

class EventHandler
{
private:
    EventCallback callback;
public:
    EventHandler();
    ~EventHandler();
};

#endif
