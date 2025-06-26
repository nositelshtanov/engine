#include "ProcessManager.h"

ProcessManager::ProcessManager(EventBus &eventBus)
    : m_eventBus(eventBus)
{
}

void ProcessManager::PushProcess(IProcess *pr)
{
    if (!m_stack.empty())
        DeactivateProcess(m_stack.back());
    m_stack.push_back(pr);
    ActivateProcess(pr);
}

IProcess *ProcessManager::PopProcess()
{
    auto *pr = m_stack.back();
    DeactivateProcess(pr);
    m_stack.pop_back();
    if (!m_stack.empty())
        ActivateProcess(m_stack.back());
    return pr;
}

void ProcessManager::FinishWork()
{
}

void ProcessManager::DeactivateProcess(IProcess *pr)
{
    pr->UnsetFlag(IProcess::fActive);
    auto &&reqEvTypes = pr->GetIEventReceiver()->GetRequiredEventTypes();
    for (auto &&evType : reqEvTypes)
        m_eventBus.Unsubscribe(*pr->GetIEventReceiver(), evType);
}

void ProcessManager::ActivateProcess(IProcess *pr)
{
    pr->SetFlag(IProcess::fActive);
    auto &&reqEvTypes = pr->GetIEventReceiver()->GetRequiredEventTypes();
    for (auto &&evType : reqEvTypes)
        m_eventBus.Subscribe(*pr->GetIEventReceiver(), evType);
}