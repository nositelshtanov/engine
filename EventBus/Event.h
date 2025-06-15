#pragma once

enum class EventType
{
    MouseEvent,
    KeyBoardEvent,
    count
};

class Event {
public:
    Event()
    {}
    virtual EventType GetEventType() const = 0;
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
};


class KeyboardEvent : public Event
{
public:
    KeyboardEvent()
    {}
    virtual EventType GetEventType() const { return EventType::KeyBoardEvent; }
};