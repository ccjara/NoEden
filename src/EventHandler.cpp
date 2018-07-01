#include "stdafx.h"

EventHandler::EventHandler(uint32_t eventType, const EventCallback& callback) :
    eventType(eventType), 
    callback(callback),
    attached(true)
{
}

EventHandler::~EventHandler()
{
    LOG(DEBUG) << "EventHandler @" << this << " destructing...";
}

void EventHandler::detach() noexcept
{
    LOG(DEBUG) << "EventHandler @" << this << " detached.";

    attached = false;
}

bool EventHandler::isDetached() const noexcept
{
    return !attached;
}

uint32_t EventHandler::getEventType() const noexcept
{
    return eventType;
}

void EventHandler::trigger(const SDL_Event& e)
{
    LOG(DEBUG) << "EventHandler @" << this << " triggered.";

    callback(e);
}