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
    int ProcessEvents();
     
    // Подписаться на тип события (будут приходить броадкастные и по таргету). Или лучше даже воспринимать это как фильтр
    void Subscribe(EventReceiver& eventReceiver, EventType eventType);
    
    // Подписаться на конкретного эмиттера (будут приходить события, на типы которых подписан)
    void ListenToEmitter(const EventReceiver& eventReceiver, size_t emitterId);
    
    void Unsubscribe(const EventReceiver& eventReceiver, EventType eventType);
    
    void StopListening(const EventReceiver& eventReceiver, size_t emitterId);
    
    void SendEvent(std::unique_ptr<Event> event);
    
    void PostEvent(std::unique_ptr<Event> event);
    
private:
    bool ProcessBroadCast(const Event& event);
    bool ProcessEventWithTarget(const Event& event);
    bool ProcessEventWithOnlyEmitter(const Event& event);
    bool CheckEvTypesAndSendEvent(EventReceiver& receiver, const Event& event);
};

EventBus& GetEventBus();