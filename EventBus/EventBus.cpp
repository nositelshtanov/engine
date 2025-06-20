#include "EventBus.h"

size_t EventReceiver::s_lastFreeId = 0;

EventBus& GetEventBus()
{
    static EventBus* eventBus = new EventBus();
    return *eventBus;
}