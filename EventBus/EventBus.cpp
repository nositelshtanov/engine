#include "EventBus.h"

#include <iostream>

size_t EventReceiver::s_lastFreeId = 0;

int EventBus::ProcessEvents()
{
    int processedCount = 0;
    std::swap(m_eventsToProcess, m_postedEvents);
    std::swap(m_eventsWithOnlyEmitter, m_postedEventsWithOnlyEmitter);

    // добавить обработку с онли эмиттером
    while (!m_eventsToProcess.empty())
    {
        auto &&event = m_eventsToProcess[0];
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
        
        m_eventsToProcess.erase(m_eventsToProcess.begin());
    }
    m_eventsToProcess.clear();
    m_eventsWithOnlyEmitter.clear();
    return processedCount;
}

// Подписаться на тип события (будут приходить броадкастные и по таргету). Или лучше даже воспринимать это как фильтр
void EventBus::Subscribe(const std::shared_ptr<EventReceiver>& eventReceiver, EventType eventType)
{
    if (!eventReceiver)
        return;

    if (m_subscribersLocker.IsLocked()) {
        m_subscribersLocker.Subscribe(eventReceiver, eventType);
        return;
    }

    if (auto &&it = m_subscribers.find(eventReceiver->GetReceiverId()); it != m_subscribers.end())
    {
        auto &&subscription = (*it).second;
        if (subscription.eventTypes.find(eventType) == subscription.eventTypes.end())
            subscription.eventTypes.insert(eventType);
        return;
    }
    m_subscribers.insert({eventReceiver->GetReceiverId(), {eventReceiver, std::set<EventType>({eventType})}});
}

void EventBus::ListenToEmitter(const std::shared_ptr<EventReceiver> &eventReceiver, size_t emitterId)
{
    if (!eventReceiver)
        return;

    if (auto &&it = m_emittersToSubs.find(emitterId); it != m_emittersToSubs.end())
    {
        auto &&emitterSubs = (*it).second;
        if (emitterSubs.find(eventReceiver->GetReceiverId()) == emitterSubs.end())
            emitterSubs.insert(eventReceiver->GetReceiverId());
        return;
    }
    m_emittersToSubs.insert({emitterId, {eventReceiver->GetReceiverId()}});
}

void EventBus::Unsubscribe(const std::shared_ptr<EventReceiver> &eventReceiver, EventType eventType)
{
    if (!eventReceiver)
        return;

    if (m_subscribersLocker.IsLocked()) {
        m_subscribersLocker.Unsubscribe(eventReceiver, eventType);
        return;
    }

    if (m_subscribers.find(eventReceiver->GetReceiverId()) == m_subscribers.end())
        return;

    auto &&subscription = m_subscribers[eventReceiver->GetReceiverId()];
    subscription.eventTypes.erase(eventType);

    if (subscription.eventTypes.empty())
        m_subscribers.erase(eventReceiver->GetReceiverId());
}

void EventBus::StopListening(const std::shared_ptr<EventReceiver> &eventReceiver, size_t emitterId)
{
    if (!eventReceiver)
        return;

    if (m_emittersToSubs.find(emitterId) == m_emittersToSubs.end())
        return;

    auto &&subs = m_emittersToSubs[emitterId];
    subs.erase(eventReceiver->GetReceiverId());
}


void EventBus::SubscribersLocker::Subscribe(const std::shared_ptr<EventReceiver>& eventReceiver, EventType eventType)
{
    if (!eventReceiver)
        return;
    m_toSubscribe.push_back(std::make_pair(eventReceiver->GetReceiverId(), Subscription{eventReceiver, {eventType}}));
}

void EventBus::SubscribersLocker::Unsubscribe(const std::shared_ptr<EventReceiver> &eventReceiver, EventType eventType)
{
    if (!eventReceiver)
        return;
    m_toUnsubscribe.push_back(std::make_pair(eventReceiver->GetReceiverId(), Subscription{eventReceiver, {eventType}}));
}

void EventBus::SubscribersLocker::Unlock() {
    m_isLocked = false;
    for (auto && [subscriberId, subscription]: m_toSubscribe) {
        m_eventBus.Subscribe(subscription.receiver, *subscription.eventTypes.begin());
    }
    for (auto && [subscriberId, subscription]: m_toUnsubscribe) {
        m_eventBus.Unsubscribe(subscription.receiver, *subscription.eventTypes.begin());
    }
}

RaiiWrapper EventBus::SubscribersLocker::CreateLock(SubscribersLocker& subscribersLocker) {
    auto onCreate = [&subscribersLocker]() {
        subscribersLocker.Lock();
    };
    auto onDelete = [&subscribersLocker]() {
        subscribersLocker.Unlock();
    };
    return RaiiWrapper(onCreate, onDelete);
}


void EventBus::SendEvent(std::unique_ptr<Event> event)
{
    if (event && !event->HasRecipients() && !event->IsBroadcast())
    {
        m_eventsWithOnlyEmitter.emplace(event->GetEventEmitterId(), std::move(event));
        return;
    }
    m_eventsToProcess.push_back(std::move(event));
}

void EventBus::PostEvent(std::unique_ptr<Event> event)
{
    if (event && !event->HasRecipients() && !event->IsBroadcast())
    {
        m_postedEventsWithOnlyEmitter.emplace(event->GetEventEmitterId(), std::move(event));
        return;
    }

    m_postedEvents.push_back(std::move(event));
}

bool EventBus::ProcessBroadCast(const Event &event)
{
    auto && lock = SubscribersLocker::CreateLock(m_subscribersLocker);
    for (auto &&[subscriberId, subscription] : m_subscribers)
        if (auto && receiver = subscription.receiver)
            CheckEvTypesAndSendEvent(*receiver, event);

    return true;
}

bool EventBus::ProcessEventWithTarget(const Event &event)
{
    auto && lock = SubscribersLocker::CreateLock(m_subscribersLocker);
    auto &&recipients = event.GetRecipients();
    for (auto &&recipientId : recipients)
    {
        if (m_subscribers.find(recipientId) != m_subscribers.end())
            if (auto && eventReceiver = m_subscribers[recipientId].receiver)
                CheckEvTypesAndSendEvent(*eventReceiver, event);
    }
    return true;
}

bool EventBus::ProcessEventWithOnlyEmitter(const Event &event)
{
    auto && lock = SubscribersLocker::CreateLock(m_subscribersLocker);
    auto &&subs = m_emittersToSubs[event.GetEventEmitterId()];
    for (auto &&subId : subs)
    {
        if (auto && receiver = m_subscribers[subId].receiver)
            CheckEvTypesAndSendEvent(*receiver, event);
    }
    return true;
}

bool EventBus::CheckEvTypesAndSendEvent(EventReceiver &receiver, const Event &event)
{
    auto &&reqTypes = receiver.GetRequiredEventTypes();
    if (reqTypes.find(event.GetEventType()) != reqTypes.end())
        return receiver.ReceiveEvent(event);
    return false;
}


EventBus &GetEventBus()
{
    static EventBus *eventBus = new EventBus();
    return *eventBus;
}