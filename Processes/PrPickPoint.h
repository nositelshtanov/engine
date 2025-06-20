#pragma once

#include <optional>

#include "../Editor3D.h"
#include "ProcessBase.h"
#include "../Math/Base.h"

class PrPickPoint : public ProcessBase
{
    std::optional<MVertex> m_point; 
public:
    using BaseClass = ProcessBase;

    PrPickPoint(PrIds id, IProcess * parent, Editor3D& editor)
        : ProcessBase(id, parent, editor)
    {}

    virtual bool Run()
    {
        return BaseClass::Run();
    }

    virtual bool Stop() 
    {
        if (!m_point.has_value())
            SetFlag(IProcess::fCancelled);
        return BaseClass::Stop();
    }

    virtual bool ReceiveEvent(const Event& event) 
    {
        if (event.GetEventType() != EventType::MouseEvent)
            return false;

        const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(event);
        if (mouseEvent.GetAction() == MouseEvent::Action::Press && mouseEvent.GetBtn() == MouseEvent::Button::Left)
        {
            m_point = MVertex(mouseEvent.GetX(), mouseEvent.GetY(), 0);
            SetFlag(IProcess::fDone);
            return true;
        } 
        return false;
    }

    virtual std::unique_ptr<IPrResult> GetPrResult() const 
    {
        if (!m_point.has_value())
            return nullptr;
        
        return std::make_unique<PrPointResult>(m_point.value());
    }

    virtual std::string GetCurStateHint() const 
    {
        return "Укажите 2д точку";
    }

    virtual void CancelCurState() 
    {
        Stop();
    }

    virtual std::set<EventType> GetRequiredEventTypes() const
    {
        return {EventType::MouseEvent};
    }
};
