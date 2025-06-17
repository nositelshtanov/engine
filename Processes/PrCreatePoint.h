#pragma once

#include "PrResult.h"
#include "PrPickPoint.h"

class PrCreatePoint : public ProcessBase
{
public:
    using BaseClass = ProcessBase;

    PrCreatePoint(PrId id, IProcess * parent)
        : ProcessBase(id, parent)
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

private:
    void RunPickPointSubProcess()
    {
        auto && pr = std::make_shared<PrPickPoint>(static_cast<PrId>(PrIds::PickPoint), this);
        m_childs.emplace(pr);
        bool res = pr->Run();
    }
};