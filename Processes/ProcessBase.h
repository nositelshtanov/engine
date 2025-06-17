#pragma once

#include <set>
#include <memory>
#include <iostream>

#include "IProcess.h"
#include "ProcessManager.h"
#include "ProcessCreator.h"

class ProcessBase : public EventReceiver 
                  , public IProcess
{
    PrIds m_id;
protected:
    IProcess* m_parent;
    size_t m_flags;
    std::set<std::shared_ptr<IProcess>> m_childs;
    ProcessManager& m_prManager;
public:
    ProcessBase(PrIds id, IProcess * parent, ProcessManager& prManager)
        : EventReceiver()
        , IProcess()
        , m_id(id)
        , m_parent(parent)
        , m_flags(0)
        , m_childs()
        , m_prManager(prManager)
    {}
    virtual PrIds GetPrId() const override { return m_id; }
    
    virtual bool Run() override
    { 
        SetFlag(IProcess::fRunning);
        m_prManager.PushProcess(this);
        return true; 
    }

    virtual bool Stop() override
    { 
        UnsetFlag(IProcess::fRunning);

        if (m_prManager.PopProcess() != this)
            std::cout << "Пиздоооос!!!" << std::endl; 

        if (m_parent)
            m_parent->ChildStop(static_cast<PrIds>(GetPrId()));

        return true; 
    }

    virtual IProcess* GetParent() override { return m_parent; }
    virtual std::set<std::shared_ptr<IProcess>> GetChilds() override { return m_childs; }

    virtual bool ReceiveEvent(const Event& event) override
    { return false; }

    virtual size_t GetFlags() const override { return m_flags;}
    virtual void SetFlag(IProcess::ProcessFlags flag) { m_flags |= flag; }
    virtual void UnsetFlag(ProcessFlags flag) { m_flags &= ~flag; }
    virtual bool GetFlagVal(ProcessFlags flag) const override { return m_flags & flag; }

    virtual std::unique_ptr<IPrResult> GetPrResult() const { return nullptr; }
    virtual bool IsDone() const { return GetFlagVal(IProcess::fDone); } 
    virtual bool IsCancelled() const { return GetFlagVal(IProcess::fCancelled); }

    virtual std::set<EventType> GetRequiredEventTypes() const { return {EventType::MouseEvent, EventType::KeyBoardEvent}; }

    virtual void ChildStop(PrIds id) {}

    virtual EventReceiver * GetIEventReceiver() { return this; }

    std::shared_ptr<IProcess> RunSubProc(PrIds id)
    {
        std::shared_ptr<IProcess> pr(CreateProc(id, this, m_prManager));
        m_childs.emplace(pr);
        bool res = pr->Run();      
        return pr;
    }

    std::shared_ptr<IProcess> FindChildProc(PrIds id)
    {
        for (auto && pr : m_childs)
        {
            if (pr->GetPrId() == id)
                return pr;
        }
        return nullptr;
    }

    virtual ~ProcessBase() override = default;
};