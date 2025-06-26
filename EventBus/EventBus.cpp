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
void EventBus::Subscribe(EventReceiver &eventReceiver, EventType eventType)
{
    if (auto &&it = m_subscribers.find(eventReceiver.GetReceiverId()); it != m_subscribers.end())
    {
        auto &&subscription = (*it).second;
        if (subscription.eventTypes.find(eventType) == subscription.eventTypes.end())
            subscription.eventTypes.insert(eventType);
        return;
    }
    m_subscribers.insert({eventReceiver.GetReceiverId(), {&eventReceiver, std::set<EventType>({eventType})}});
}

void EventBus::ListenToEmitter(const EventReceiver &eventReceiver, size_t emitterId)
{
    if (auto &&it = m_emittersToSubs.find(emitterId); it != m_emittersToSubs.end())
    {
        auto &&emitterSubs = (*it).second;
        if (emitterSubs.find(eventReceiver.GetReceiverId()) == emitterSubs.end())
            emitterSubs.insert(eventReceiver.GetReceiverId());
        return;
    }
    m_emittersToSubs.insert({emitterId, {eventReceiver.GetReceiverId()}});
}

void EventBus::Unsubscribe(const EventReceiver &eventReceiver, EventType eventType)
{
    if (m_subscribers.find(eventReceiver.GetReceiverId()) == m_subscribers.end())
        return;

    auto &&subscription = m_subscribers[eventReceiver.GetReceiverId()];
    subscription.eventTypes.erase(eventType);
}

void EventBus::StopListening(const EventReceiver &eventReceiver, size_t emitterId)
{
    if (m_emittersToSubs.find(emitterId) == m_emittersToSubs.end())
        return;

    auto &&subs = m_emittersToSubs[emitterId];
    subs.erase(eventReceiver.GetReceiverId());
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
    if (event)
    {
        switch (event->GetEventType())
        {
        case EventType::KeyBoardEvent:
            {
            KeyboardEvent* ev = static_cast<KeyboardEvent*>(event.get());
            if (ev->GetKey() == KeyboardEvent::Keys::P)
                std::cout << "P key event" << std::endl;
            else 
                std::cout << "other key event" << std::endl;
            break;
            }
        case EventType::MouseEvent:
            {
            std::cout << "mouse event" << std::endl;
            break;
            }
        default:
        {
            break;
        }
        }
    }
    m_postedEvents.push_back(std::move(event));
}

bool EventBus::ProcessBroadCast(const Event &event)
{
    for (auto &&[subscriberId, subscription] : m_subscribers)
        if (auto *receiver = subscription.receiver)
            CheckEvTypesAndSendEvent(*receiver, event);
    return true;
}

bool EventBus::ProcessEventWithTarget(const Event &event)
{
    auto &&recipients = event.GetRecipients();
    for (auto &&recipientId : recipients)
    {
        if (m_subscribers.find(recipientId) != m_subscribers.end())
            if (auto *eventReceiver = m_subscribers[recipientId].receiver)
                CheckEvTypesAndSendEvent(*eventReceiver, event);
    }
    return true;
}

bool EventBus::ProcessEventWithOnlyEmitter(const Event &event)
{
    auto &&subs = m_emittersToSubs[event.GetEventEmitterId()];
    for (auto &&subId : subs)
    {
        if (auto *receiver = m_subscribers[subId].receiver)
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