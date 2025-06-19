#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <memory>

#include "Event.h"
#include "EventReceiver.h"


class EventBus
{
    std::vector<std::unique_ptr<Event>> m_eventsToProcess; 
    std::vector<std::unique_ptr<Event>> m_postedEvents;

    std::unordered_map<size_t, std::unique_ptr<Event>> m_eventsWithOnlyEmitter;
    std::unordered_map<size_t, std::unique_ptr<Event>> m_postedEventsWithOnlyEmitter;

    struct Subscription
    {
        EventReceiver* receiver; // сам подписчик
        std::set<EventType> eventTypes; // типы событий которые хочет получать
        //std::vector<size_t> subsByEmitter; // эмиттеры на которых подписан
    };

    std::unordered_map<size_t, Subscription> m_subscribers; 
    std::unordered_map<size_t, std::set<size_t>> m_emittersToSubs; // эмиттер - его подписчики
public:
    EventBus()
        : m_eventsToProcess()
        , m_postedEvents()
        , m_eventsWithOnlyEmitter()
        , m_postedEventsWithOnlyEmitter()
        , m_subscribers()
    {}
    int ProcessEvents()
    {
        int processedCount = 0;
        std::swap(m_eventsToProcess, m_postedEvents);
        std::swap(m_eventsWithOnlyEmitter, m_postedEventsWithOnlyEmitter);
        size_t i = 0;
        while (!m_eventsToProcess.empty()) 
        {
            auto && event = m_eventsToProcess[i];
            if (!event)
                continue;
                
            bool res = false;
                
            if (event->IsBroadcast())
                res = ProcessBroadCast(*event);     
            else if (event->HasRecipients())
                res = ProcessEventWithTarget(*event);
            else if (event->HasRecipients())
                res = ProcessEventWithOnlyEmitter(*event);

            if (res)
                ++processedCount;
        }
        m_eventsToProcess.clear();
        m_eventsWithOnlyEmitter.clear();
        return processedCount; 
    } 
    void Subscribe(EventReceiver& eventReceiver, EventType eventType)
    {
        if (auto && it = m_subscribers.find(eventReceiver.GetReceiverId()); it != m_subscribers.end())
        {
            auto && subscription = (*it).second;
            if (subscription.eventTypes.find(eventType) == subscription.eventTypes.end())
                subscription.eventTypes.insert(eventType);
            return;
        }
        m_subscribers.insert({eventReceiver.GetReceiverId(), {&eventReceiver, std::set<EventType>({eventType})}});
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
private:
    bool ProcessBroadCast(const Event& event)
    {
        for (auto && [subscriberId, subscription] : m_subscribers)
            if (auto * receiver = subscription.receiver)
                CheckEvTypesAndSendEvent(*receiver, event); 
        return true;
    }
    bool ProcessEventWithTarget(const Event& event)
    {
        auto && recipients = event.GetRecipients();
        for (auto && recipientId : recipients)
        {
            if (m_subscribers.find(recipientId) != m_subscribers.end())
                if (auto * eventReceiver = m_subscribers[recipientId].receiver)
                    CheckEvTypesAndSendEvent(*eventReceiver, event); 
        }
        return true;
    }
    bool ProcessEventWithOnlyEmitter(const Event& event)
    {
        auto && subs = m_emittersToSubs[event.GetEventEmitterId()];
        for (auto && subId : subs)
        {
            if (auto * receiver = m_subscribers[subId].receiver)
                CheckEvTypesAndSendEvent(*receiver, event);
        }
        return true;
    }
    bool CheckEvTypesAndSendEvent(EventReceiver& receiver, const Event& event)
    {
        auto && reqTypes = receiver.GetRequiredEventTypes();
            if (reqTypes.find(event.GetEventType()) != reqTypes.end())
                return receiver.ReceiveEvent(event);
        return false;
    }
};