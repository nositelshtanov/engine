#pragma once

#include "../Editor3D.h"
#include "ProcessCreator.h"
#include "PrPickPoint.h"
#include "../EventBus/Event.h"


class PrMain : public ProcessBase
{
public:
    using BaseClass = ProcessBase;

    PrMain(Editor3D& editor)
       : ProcessBase(PrIds::Main, nullptr, editor) 
    {}

    virtual bool Run() override
    { 
        bool res = BaseClass::Run();        
        return res; 
    }

    virtual bool Stop() override
    { 
        bool res = BaseClass::Stop();
        for (auto && child : m_childs)
            child->Stop(); 
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

    virtual void ChildStop(PrIds id) 
    {
        auto && child = FindChildProc(id);
        if (!child)
            return;
        if (child->IsDone())
        {
            auto && result = child->GetPrResult();
        }
        m_childs.erase(child);
    }

private:
    void RunSubProcess(PrIds id)
    {
        if (id == PrIds::PickPoint)
        {
            std::shared_ptr<IProcess> pr(CreateProc(PrIds::PickPoint, this, m_editor));
            m_childs.emplace(pr);
            bool res = pr->Run();
        }
    }
    
};