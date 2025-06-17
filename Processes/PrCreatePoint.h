#pragma once

#include "PrResult.h"
#include "ProcessManager.h"
#include "ProcessCreator.h"
#include "ProcessBase.h"

class PrCreatePoint : public ProcessBase
{
public:
    using BaseClass = ProcessBase;

    PrCreatePoint(PrIds id, IProcess * parent, ProcessManager& prManager)
        : ProcessBase(id, parent, prManager)
    {}

    virtual bool Run()
    {
        bool res = BaseClass::Run();
        RunPickPointSubProcess();
        return res;
    }
    
    virtual bool Stop()
    {
        for (auto && child : m_childs)
            child->Stop();
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

    virtual void ChildStop(PrIds id) 
    {
        if (id != PrIds::PickPoint)
            return;
        
        auto && child = FindChildProc(id);
        if (!child)
            return;
        if (child->IsDone())
        {
            auto && result = child->GetPrResult();
            if (result->GetType() == PrResultType::Point)
            {
                auto && pointResult = static_cast<PrPointResult&>(*result);
                // TODO: Create Object "Point", write GetPrResult
            }
        }
        m_childs.erase(child);
    }

private:
    void RunPickPointSubProcess()
    {
        RunSubProc(PrIds::PickPoint); 
    }
};
