#pragma once

#include "PrPickPoint.h"
#include "../EventBus/Event.h"


class PrMain : public ProcessBase
{
    std::vector<IProcess*> m_processesStack;
    EventBus& m_eventBus;
public:
    using BaseClass = ProcessBase;

    PrMain(EventBus& eventBus)
       : ProcessBase(static_cast<PrId>(PrIds::Main), nullptr) 
       , m_processesStack()
       , m_eventBus(eventBus)
    {}

    virtual bool Run() override
    { 
        bool res = BaseClass::Run();        
        return res; 
    }

    virtual bool Stop() override
    { 
        bool res = BaseClass::Stop();
        return res;
    }

    virtual bool ReceiveEvent(const Event& event) override
    {
        auto et = event.GetEventType();
        switch (et)
        {
        case EventType::MouseEvent:
            {
                break;
            }
        case EventType::KeyBoardEvent:
            {
                auto && keyboardEvent = static_cast<const KeyboardEvent&>(event);
                if (keyboardEvent.GetKey() == KeyboardEvent::P)
                    RunSubProcess(PrIds::PickPoint);
                break;
            }
        default:
            {
                return false;
            }
        }
        return false;
    }

    virtual std::set<EventType> GetRequiredEventTypes() const 
    { 
        return {EventType::MouseEvent, EventType::KeyBoardEvent}; 
    }

    virtual std::unique_ptr<IPrResult> GetPrResult() const { return nullptr; }
    virtual bool IsDone() const { return false; }
    virtual bool IsCancelled() const { return false; }
    virtual std::string GetCurStateHint() const { return ""; }
    virtual void CancelCurState() {} 

    IProcess* GetActiveProcess() const { return !m_processesStack.empty() ? m_processesStack.back() : nullptr; }

    virtual void ChildStop(PrIds id) 
    {
        auto && child = FindChildProc(id);
        if (!child)
            return;
        if (child->IsDone())
        {
            auto && result = child->GetPrResult();
            child->UnsetFlag(IProcess::fActive);
            m_processesStack.pop_back();
            m_childs.erase(child);
            auto && reqEvTypes = child->GetRequiredEventTypes();
            for (auto && evType : reqEvTypes)
                m_eventBus.Subscribe(*child->GetIEventReceiver(), evType);
        }
    }

private:
    std::shared_ptr<IProcess> FindChildProc(PrIds id)
    {
        for (auto && pr : m_childs)
        {
            if (pr->GetPrId() == static_cast<PrId>(id))
                return pr;
        }
        return nullptr;
    }
    void RunSubProcess(PrIds id)
    {
        if (id == PrIds::PickPoint)
        {
            auto && pr = std::make_shared<PrPickPoint>(static_cast<PrId>(id), this);
            m_childs.emplace(pr);
            bool res = pr->Run();
            pr->SetFlag(IProcess::fActive);
            m_processesStack.push_back(pr.get());
            auto && reqEvTypes = pr->GetRequiredEventTypes();
            for (auto && evType : reqEvTypes)
                m_eventBus.Subscribe(*pr, evType);    
        }
    }
    
};