#pragma once

#include <stddef.h>
#include <set>

#include "Event.h"

class EventReceiver
{
    static size_t s_lastFreeId;
    size_t m_receiverId;
public:
    EventReceiver()
        : m_receiverId(s_lastFreeId++)
    {}
    size_t GetReceiverId() const { return m_receiverId; }
    virtual bool ReceiveEvent(const Event& event) = 0;
    virtual std::set<EventType> GetRequiredEventTypes() const = 0;
    virtual ~EventReceiver() = default;
};