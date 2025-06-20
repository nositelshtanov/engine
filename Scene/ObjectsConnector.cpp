#include "ObjectsConnector.h"

#include "../EventBus/Event.h"
#include "../EventBus/EventReceiver.h"
#include "../EventBus/EventBus.h"

/// подписать себя на тип событий
void ObjectsConnector::Subscribe(EventReceiver& eventReceiver, EventType eventType)
{
    GetEventBus().Subscribe(eventReceiver, eventType);
};

void ObjectsConnector::Unsubscribe(const EventReceiver& eventReceiver, EventType eventType)
{
    GetEventBus().Unsubscribe(eventReceiver, eventType);
};

/// Подписаться на эмиттера
void ObjectsConnector::ListenToEmitter(const EventReceiver& eventReceiver, size_t emitterId)
{
    GetEventBus().ListenToEmitter(eventReceiver, emitterId);
}

void ObjectsConnector::StopListening(const EventReceiver& eventReceiver, size_t emitterId)
{
    GetEventBus().StopListening(eventReceiver, emitterId);
}