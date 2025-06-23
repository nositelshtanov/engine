#pragma once

#include <memory>
#include <stddef.h>

class EventReceiver;
enum class EventType;
class Event;

class ObjectsConnector
{
public:
    /// подписать себя на тип событий
    static void Subscribe(EventReceiver&, EventType);
    static void Unsubscribe(const EventReceiver&, EventType);
    /// Подписаться на эмиттера
    static void ListenToEmitter(const EventReceiver&, size_t);
    static void StopListening(const EventReceiver&, size_t);
    /// запушить событие в текущую итерацию
    static void SendEvent(std::unique_ptr<Event> event);
    /// запушить событие в след. итерацию
    static void PostEvent(std::unique_ptr<Event> event);
};