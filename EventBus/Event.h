#pragma once

#include <stddef.h>
#include <limits>
#include <vector>

enum class EventType
{
    MouseEvent,
    KeyBoardEvent,
    ObjRebuild,
    count
};

class Event {
    size_t m_eventEmmiterId;
    std::vector<size_t> m_recipients;
public:
    Event(size_t emitterId, std::vector<size_t> recipietns = {})
        : m_eventEmmiterId(emitterId)
        , m_recipients(recipietns)
    {}
    Event()
        : m_eventEmmiterId(std::numeric_limits<size_t>::max())
    {}
    virtual bool HasEventEmitter() const { return m_eventEmmiterId != std::numeric_limits<size_t>::max(); }
    virtual size_t GetEventEmitterId() const { return m_eventEmmiterId; }
    virtual bool HasRecipients() const { return !m_recipients.empty(); }
    virtual std::vector<size_t> GetRecipients() const { return m_recipients; }
    virtual EventType GetEventType() const = 0;
    virtual bool IsBroadcast() const { return false; }
};

class MouseEvent : public Event 
{
public:
    enum class Button {
        Left,
        Right,
        Middle
    };
    enum class Action {
        Press,
        Release,
        Move,
        Scroll
    };
private:
    Button m_btn;
    Action m_action;
    double m_x;
    double m_y;
public:
    MouseEvent(double x, double y, Button btn, Action action)
        : m_btn(btn)
        , m_action(action)
        , m_x(x)
        , m_y(y)
    {}
    virtual EventType GetEventType() const { return EventType::MouseEvent; }
    double GetX() const { return m_x; }
    double GetY() const { return m_y; }
    Button GetBtn() const { return m_btn; }
    Action GetAction() const { return m_action; }
    virtual bool IsBroadcast() const { return true; }
};


class KeyboardEvent : public Event
{
public:
    enum Keys 
    {
        P,
        M,
        E,
        count
    };
private:
    Keys m_key;

public:
    KeyboardEvent(Keys key)
        : m_key(key)
    {}
    virtual EventType GetEventType() const { return EventType::KeyBoardEvent; }
    Keys GetKey() const { return m_key; }

    virtual bool IsBroadcast() const { return true; }
};


class ObjRebuildEvent : public Event
{
public:
    ObjRebuildEvent(size_t emitter)
        : Event(emitter)
    {}
    ObjRebuildEvent(size_t emitter, std::vector<size_t> recepients)
        : Event(emitter, recepients)
    {}
    virtual EventType GetEventType() const { return EventType::ObjRebuild; }
};