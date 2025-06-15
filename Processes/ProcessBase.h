#pragma once

#include "IProcess.h"
#include "../EventBus/EventBus.h"

class ProcessBase : public EventReceiver
                  , public IProcess
{
    PrId m_id;
public:
    ProcessBase(PrId id)
        : EventReceiver()
        , IProcess()
        , m_id(id)
    {}
    virtual PrId GetPrId() const override { return m_id; }
    
    virtual bool Run() override
    { return true; }

    virtual bool Stop() override
    { return true; }

    virtual bool ReceiveEvent(const Event& event) override
    { return false; }
}