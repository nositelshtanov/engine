#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <memory>

#include "Event.h"
#include "EventReceiver.h"
#include "../Util.h"


class EventBus
{
    std::vector<std::unique_ptr<Event>> m_eventsToProcess; 
    std::vector<std::unique_ptr<Event>> m_postedEvents;

    std::unordered_map<size_t, std::unique_ptr<Event>> m_eventsWithOnlyEmitter;
    std::unordered_map<size_t, std::unique_ptr<Event>> m_postedEventsWithOnlyEmitter;

    struct Subscription
    {
        std::shared_ptr<EventReceiver> receiver; // сам подписчик
        std::set<EventType> eventTypes; // типы событий которые хочет получать
        //std::vector<size_t> subsByEmitter; // эмиттеры на которых подписан
    };

    using Subscribers = std::unordered_map<size_t, Subscription>;
    Subscribers m_subscribers;
    std::unordered_map<size_t, std::set<size_t>> m_emittersToSubs; // эмиттер - его подписчики

public:
    class SubscribersLocker {
        EventBus& m_eventBus;
        bool m_isLocked;
        struct SubEvent {
            Subscription subscription;
            bool isSub; // подписка или отписка
        };
        std::vector<SubEvent> m_subEvents;
        //std::vector<std::pair<size_t, Subscription>> m_toSubscribe;
        //std::vector<std::pair<size_t, Subscription>> m_toUnsubscribe;
    public:
        SubscribersLocker(EventBus& eventBus)
            : m_eventBus(eventBus)
            , m_isLocked(false)
            , m_subEvents()
            //, m_toSubscribe()
            //, m_toUnsubscribe()
        {}

        static RaiiWrapper CreateLock(SubscribersLocker& subscribersLocker);

        void Lock() {
            m_isLocked = true;
        }
        bool IsLocked() const {
            return m_isLocked;
        }

        void Subscribe(const std::shared_ptr<EventReceiver>& eventReceiver, EventType eventType);
        void Unsubscribe(const std::shared_ptr<EventReceiver>& eventReceiver, EventType eventType);
        void Unlock();
    };
private:

    SubscribersLocker m_subscribersLocker;

    // Мб когда нибудь придется разделить типы событий broadcastные, по таргетам и по эмиттеру
public:
    EventBus()
        : m_eventsToProcess()
        , m_postedEvents()
        , m_eventsWithOnlyEmitter()
        , m_postedEventsWithOnlyEmitter()
        , m_subscribers()
        , m_subscribersLocker(*this)
    {}
    int ProcessEvents();
     
    // Подписаться на тип события (будут приходить броадкастные и по таргету). Или лучше даже воспринимать это как фильтр
    void Subscribe(const std::shared_ptr<EventReceiver>& eventReceiver, EventType eventType);
    
    // Подписаться на конкретного эмиттера (будут приходить события, на типы которых подписан)
    void ListenToEmitter(const std::shared_ptr<EventReceiver>& eventReceiver, size_t emitterId);
    
    void Unsubscribe(const std::shared_ptr<EventReceiver>& eventReceiver, EventType eventType);
    
    void StopListening(const std::shared_ptr<EventReceiver>& eventReceiver, size_t emitterId);
    
    void SendEvent(std::unique_ptr<Event> event);
    
    void PostEvent(std::unique_ptr<Event> event);

private:
    bool ProcessBroadCast(const Event& event);
    bool ProcessEventWithTarget(const Event& event);
    bool ProcessEventWithOnlyEmitter(const Event& event);
    bool CheckEvTypesAndSendEvent(EventReceiver& receiver, const Event& event);
};

EventBus& GetEventBus();