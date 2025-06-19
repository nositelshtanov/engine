#pragma once

#include "IObject3d.h"
#include "../EventBus/EventReceiver.h"

class Object3DBase : public IObject3D
                   , public EventReceiver
{
    size_t m_id;

    static size_t s_lastUnusedId;
public:
    Object3DBase()
        : m_id(s_lastUnusedId++)
    {}
    size_t GetObjId() const { return m_id; } 
    virtual EventReceiver* GetIEventReceiver() override 
    {
        return this;
    }

    virtual bool ReceiveEvent(const Event& event) override
    {
        return false;
    }
    virtual std::set<EventType> GetRequiredEventTypes() const override
    {
        return {};
    }

    virtual ~Object3DBase() override = default;
};