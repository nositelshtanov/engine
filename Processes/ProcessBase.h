#pragma once

#include <set>
#include <memory>

#include "IProcess.h"
#include "../EventBus/EventBus.h"

class ProcessBase : public EventReceiver
                  , public IProcess
{
    PrId m_id;
    IProcess* m_parent;
    size_t m_flags;
    std::set<std::shared_ptr<IProcess>> m_childs;
public:
    ProcessBase(PrId id, IProcess * parent)
        : EventReceiver()
        , IProcess()
        , m_id(id)
        , m_parent(parent)
        , m_flags(0)
        , m_childs()
    {}
    virtual PrId GetPrId() const override { return m_id; }
    
    virtual bool Run() override
    { 
        SetFlag(IProcess::fRunning);
        return true; 
    }

    virtual bool Stop() override
    { 
        UnsetFlag(IProcess::fRunning);
        return true; 
    }

    virtual IProcess* GetParent() override { return m_parent; }
    virtual std::set<std::shared_ptr<IProcess>> GetChilds() override { return m_childs; }

    virtual bool ReceiveEvent(const Event& event) override
    { return false; }

    virtual size_t GetFlags() const override { return m_flags;}
    virtual void SetFlag(IProcess::ProcessFlags flag) { m_flags |= flag; }
    virtual void UnsetFlag(ProcessFlags flag) { m_flags &= ~flag; }
    virtual bool GetFlagVal(ProcessFlags flag) override { return m_flags & flag; }
};