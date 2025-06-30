#include "PrMain.h"

PrMain::PrMain(Editor3D& editor)
    : ProcessBase(PrIds::Main, nullptr, editor) 
{}

bool PrMain::Run()
{
    bool res = BaseClass::Run();
    return res;
}

bool PrMain::Stop()
{
    for (auto &&child : m_childs)
        child->Stop();
    return BaseClass::Stop();
}

bool PrMain::ReceiveEvent(const Event &event)
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
        auto &&keyboardEvent = static_cast<const KeyboardEvent &>(event);
        if (keyboardEvent.GetKey() == KeyboardEvent::P && !FindChildProc(PrIds::CreatePoint))
            RunSubProcess(PrIds::CreatePoint);
        break;
    }
    default:
    {
        return false;
    }
    }
    return false;
}

std::set<EventType> PrMain::GetRequiredEventTypes() const
{
    return {EventType::MouseEvent, EventType::KeyBoardEvent};
}

std::unique_ptr<IPrResult> PrMain::GetPrResult() const 
{ 
    return nullptr; 
}

bool PrMain::IsDone() const 
{ 
    return false; 
}

bool PrMain::IsCancelled() const 
{ 
    return false; 
}

std::string PrMain::GetCurStateHint() const 
{ 
    return "P - create point; M - move something; ";
}

void PrMain::CancelCurState() 
{

}

void PrMain::ChildStop(PrIds id)
{
    auto &&child = FindChildProc(id);
    if (!child)
        return;
    if (child->IsDone())
    {
        auto &&result = child->GetPrResult();
    }
    m_childs.erase(child);
}

void PrMain::Activated() {
    BaseClass::Activated();
}

void PrMain::Deactivated() {
    BaseClass::Deactivated();
}

void PrMain::RunSubProcess(PrIds id)
{
    if (id == PrIds::CreatePoint)
    {
        auto && pr = CreateProc(PrIds::CreatePoint, this, m_editor);
        m_childs.emplace(pr);
        bool res = pr->Run();
    }
}