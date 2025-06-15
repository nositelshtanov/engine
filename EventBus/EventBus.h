#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <memory>

#include "Event.h"

class EventReceiver
{
    static size_t s_lastFreeId;
    size_t m_receiverId;
public:
    EventReceiver()
        : m_receiverId(s_lastFreeId++)
    {}
    size_t GetReceiverId() const { return m_receiverId; }
    virtual bool ReceiveEvent(const Event& event) = 0;
};

class EventBus
{
    std::vector<std::unique_ptr<Event>> m_eventsToProcess; 
    std::vector<std::unique_ptr<Event>> m_postedEvents;

    using Subscription = std::pair<EventReceiver*, std::set<EventType>>;

    std::unordered_map<size_t, Subscription> m_subscribers; 
public:
    EventBus()
        : m_eventsToProcess()
        , m_postedEvents()
        , m_subscribers()
    {}
    int ProcessEvents()
    {
        int processedCount = 0;
        std::swap(m_eventsToProcess, m_postedEvents);
        size_t i = 0;
        while (!m_eventsToProcess.empty()) 
        {
            auto && event = m_eventsToProcess[i];
            if (!event)
                continue;
                
            for (auto && [subscriberId, subscription] : m_subscribers)
                if (auto * receiver = subscription.first)
                    receiver->ReceiveEvent(*event);

            ++processedCount;
        }
        m_eventsToProcess.clear();
        return processedCount; 
    } 
    void Subscribe(EventReceiver& eventReceiver, EventType eventType)
    {
        if (auto && it = m_subscribers.find(eventReceiver.GetReceiverId()); it != m_subscribers.end())
        {
            auto && subscription = (*it).second;
            if (subscription.second.find(eventType) == subscription.second.end())
                subscription.second.insert(eventType);
            return;
        }
        m_subscribers.insert({eventReceiver.GetReceiverId(), std::make_pair(&eventReceiver, std::set<EventType>({eventType}))});
    }
    void Unsubscribe(const EventReceiver& eventReceiver, EventType eventType)
    { 
    }
    void SendEvent(std::unique_ptr<Event> event)
    {
        m_eventsToProcess.push_back(std::move(event));
    }
    void PostEvent(std::unique_ptr<Event> event) 
    {
        m_postedEvents.push_back(std::move(event));
    }
};