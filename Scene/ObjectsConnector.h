#pragma once

#include <stddef.h>

class EventReceiver;
enum class EventType;

class ObjectsConnector
{
public:
    /// подписать себя на тип событий
    static void Subscribe(EventReceiver&, EventType);
    static void Unsubscribe(const EventReceiver&, EventType);
    /// Подписаться на эмиттера
    static void ListenToEmitter(const EventReceiver&, size_t);
    static void StopListening(const EventReceiver&, size_t);
};