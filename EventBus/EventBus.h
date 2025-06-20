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

    // Мб когда нибудь придется разделить типы событий broadcastные, по таргетам и по эмиттеру
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
    // Подписаться на тип события (будут приходить броадкастные и по таргету). Или лучше даже воспринимать это как фильтр
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
    // Подписаться на конкретного эмиттера (будут приходить события, на типы которых подписан)
    void ListenToEmitter(const EventReceiver& eventReceiver, size_t emitterId)
    {
        if (auto && it = m_emittersToSubs.find(emitterId); it != m_emittersToSubs.end())
        {
            auto && emitterSubs = (*it).second; 
            if (emitterSubs.find(eventReceiver.GetReceiverId()) == emitterSubs.end())
                emitterSubs.insert(eventReceiver.GetReceiverId());
            return;
        }
        m_emittersToSubs.insert({emitterId, {eventReceiver.GetReceiverId()}});
        
    }
    void Unsubscribe(const EventReceiver& eventReceiver, EventType eventType)
    { 
        if (m_subscribers.find(eventReceiver.GetReceiverId()) == m_subscribers.end())
            return;
        
        auto && subscription = m_subscribers[eventReceiver.GetReceiverId()];
        subscription.eventTypes.erase(eventType);
    }
    void StopListening(const EventReceiver& eventReceiver, size_t emitterId)
    {
        if (m_emittersToSubs.find(emitterId) == m_emittersToSubs.end())
            return;

        auto && subs = m_emittersToSubs[emitterId];
        subs.erase(eventReceiver.GetReceiverId());
    }
    void SendEvent(std::unique_ptr<Event> event)
    {
        if (event && !event->HasRecipients() && !event->IsBroadcast())
        {
            m_eventsWithOnlyEmitter.emplace(event->GetEventEmitterId(), std::move(event));
            return;
        }
        m_eventsToProcess.push_back(std::move(event));
    }
    void PostEvent(std::unique_ptr<Event> event) 
    {
        if (event && !event->HasRecipients() && !event->IsBroadcast())
        {
            m_postedEventsWithOnlyEmitter.emplace(event->GetEventEmitterId(), std::move(event));
            return;
        }
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

EventBus& GetEventBus();