#include "ProcessBase.h"

#include <iostream>

ProcessBase::ProcessBase(PrIds id, IProcess *parent, Editor3D &editor)
    : EventReceiver(), IProcess(), m_id(id), m_parent(parent), m_flags(0), m_childs(), m_editor(editor)
{
}

PrIds ProcessBase::GetPrId() const
{
    return m_id;
}

bool ProcessBase::Run()
{
    SetFlag(IProcess::fRunning);
    m_editor.GetPrManager().PushProcess(this);
    return true;
}

bool ProcessBase::Stop()
{
    UnsetFlag(IProcess::fRunning);

    if (m_editor.GetPrManager().PopProcess() != this)
        std::cout << "Пиздоооос!!!" << std::endl;

    if (m_parent)
        m_parent->ChildStop(static_cast<PrIds>(GetPrId()));

    return true;
}

IProcess *ProcessBase::GetParent() 
{ 
    return m_parent; 
}

std::set<std::shared_ptr<IProcess>> ProcessBase::GetChilds() 
{ 
    return m_childs; 
}

bool ProcessBase::ReceiveEvent(const Event &event)
{
    return false;
}

size_t ProcessBase::GetFlags() const 
{ 
    return m_flags; 
}

void ProcessBase::SetFlag(IProcess::ProcessFlags flag) 
{ 
    m_flags |= flag; 
}

void ProcessBase::UnsetFlag(ProcessFlags flag) 
{ 
    m_flags &= ~flag; 
}

bool ProcessBase::GetFlagVal(ProcessFlags flag) const 
{ 
    return m_flags & flag; 
}

std::unique_ptr<IPrResult> ProcessBase::GetPrResult() const 
{ 
    return nullptr; 
}

bool ProcessBase::IsDone() const 
{ 
    return GetFlagVal(IProcess::fDone); 
}

bool ProcessBase::IsCancelled() const 
{ 
    return GetFlagVal(IProcess::fCancelled); 
}

std::set<EventType> ProcessBase::GetRequiredEventTypes() const 
{ 
    return {EventType::MouseEvent, EventType::KeyBoardEvent}; 
}

void ProcessBase::ChildStop(PrIds id) 
{

}

EventReceiver *ProcessBase::GetIEventReceiver() 
{ 
    return this; 
}

std::shared_ptr<IProcess> ProcessBase::RunSubProc(PrIds id)
{
    std::shared_ptr<IProcess> pr(CreateProc(id, this, m_editor));
    m_childs.emplace(pr);
    bool res = pr->Run();
    return pr;
}

std::shared_ptr<IProcess> ProcessBase::FindChildProc(PrIds id)
{
    for (auto &&pr : m_childs)
    {
        if (pr->GetPrId() == id)
            return pr;
    }
    return nullptr;
}

ProcessBase::~ProcessBase() = default;