#include "event.hpp"

void EventHandler::onClick(int, int, int, int) {}
void EventHandler::onTick(int) {}
void EventHandler::onRender(int, int) {}

void EventBus::regEvent(EventHandler& handler)
{
    handlers.push_back(&handler);
}

void EventBus::onClick(int button, int state, int x, int y)
{
    for (EventHandler* handler : handlers) handler->onClick(button, state, x, y);
}

void EventBus::onTick(int tick)
{
    for (EventHandler* handler : handlers) handler->onTick(tick);
}

void EventBus::onRender(int x, int y)
{
    for (EventHandler* handler : handlers) handler->onRender(x, y);
}
