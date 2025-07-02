#include "PrPickPoint.h"

PrPickPoint::PrPickPoint(PrIds id, IProcess *parent, Editor3D &editor)
    : ProcessBase(id, parent, editor)
{
}

bool PrPickPoint::Run()
{
    return BaseClass::Run();
}

bool PrPickPoint::Stop()
{
    if (!m_point.has_value())
        SetFlag(IProcess::fCancelled);
    return BaseClass::Stop();
}

bool PrPickPoint::ReceiveEvent(const Event &event)
{
    if (event.GetEventType() != EventType::MouseEvent)
        return false;

    const MouseEvent &mouseEvent = static_cast<const MouseEvent &>(event);
    if (mouseEvent.GetAction() == MouseEvent::Action::Press && mouseEvent.GetBtn() == MouseEvent::Button::Left)
    {
        m_point = MPoint3D(mouseEvent.GetX(), mouseEvent.GetY(), 0);
        //std::cout << "Picked point: " << m_point->x << " " << m_point->y << std::endl;
        SetFlag(IProcess::fDone);
        Stop();
        return true;
    }
    return false;
}

std::unique_ptr<IPrResult> PrPickPoint::GetPrResult() const
{
    if (!m_point.has_value())
        return nullptr;

    return std::make_unique<PrPointResult>(m_point.value());
}

bool PrPickPoint::IsDone() const
{
    return m_point.has_value();
}

bool PrPickPoint::IsCancelled() const
{
    return !IsDone();
}

std::string PrPickPoint::GetCurStateHint() const
{
    return "pick point";
}

void PrPickPoint::CancelCurState()
{
    Stop();
}

std::set<EventType> PrPickPoint::GetRequiredEventTypes() const
{
    return {EventType::MouseEvent};
}
