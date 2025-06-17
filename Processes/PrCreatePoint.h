#pragma once

#include "PrResult.h"
#include "PrPickPoint.h"
#include "ProcessManager.h"

class PrCreatePoint : public ProcessBase
{
public:
    using BaseClass = ProcessBase;

    PrCreatePoint(PrIds id, IProcess * parent, ProcessManager& prManager)
        : ProcessBase(id, parent, prManager)
    {}

    virtual bool Run()
    {
        return BaseClass::Run();
    }
    
    virtual bool Stop()
    {
        return BaseClass::Stop();
    }

    virtual bool ReceiveEvent(const Event& event) 
    {
        return false;
    }

    virtual std::unique_ptr<IPrResult> GetPrResult() const 
    {
        return nullptr;
    }

    virtual void CancelCurState() 
    {
        Stop();
    }

    virtual std::set<EventType> GetRequiredEventTypes() const
    {
        return {EventType::MouseEvent};
    }
    
    virtual std::string GetCurStateHint() const 
    {
        return "";
    }

private:
    void RunPickPointSubProcess()
    {
        auto && pr = std::make_shared<PrPickPoint>(PrIds::PickPoint, this, m_prManager);
        m_childs.emplace(pr);
        bool res = pr->Run();
    }
};
