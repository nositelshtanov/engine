#pragma once

#include <vector>
#include <unordered_map>

class Event {

public:
    Event()
    {}
};

class IEventReceiver
{
public:
    size_t GetId() const 
    {}
    bool ReceiveEvent(const Event& event)
    {}
};

class EventBus
{
    std::vector<Event> m_events; 
    std::unordered_map<int/*someId например*/, IEventReceiver*> m_subscribers; 
public:
    EventBus()
        : m_events()
        , m_subscribers()
    {}
    int ProcessEvents()
    {
        int processedCount = 0;
        for (auto && event : m_events)
        {
            ++processedCount;
        }
        return processedCount; 
    } 
    bool Subscribe(const IEventReceiver& eventReceiver)
    {

    }
    bool Unsubscribe(const IEventReceiver& eventReceiver)
    {}
};