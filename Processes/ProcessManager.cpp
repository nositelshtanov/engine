#include "ProcessManager.h"

#include <iostream>

ProcessManager::ProcessManager(EventBus &eventBus)
    : m_stack()
    , m_eventBus(eventBus)
{
}

void ProcessManager::PushProcess(std::shared_ptr<IProcess> pr)
{
    if (!m_stack.empty())
        DeactivateProcess(m_stack.back());
    m_stack.push_back(pr);
    ActivateProcess(pr);
}

std::shared_ptr<IProcess> ProcessManager::PopProcess()
{
    auto && pr = m_stack.back();
    DeactivateProcess(pr);
    m_stack.pop_back();
    if (!m_stack.empty())
        ActivateProcess(m_stack.back());
    std::cout << std::endl;
    return pr;
}

void ProcessManager::FinishWork()
{
}

void ProcessManager::DeactivateProcess(std::shared_ptr<IProcess> pr)
{
    std::cout << "deactivate Proc: " << static_cast<int>(pr->GetPrId()) << std::endl;
    pr->UnsetFlag(IProcess::fActive);
    auto &&reqEvTypes = pr->GetIEventReceiver()->GetRequiredEventTypes();
    for (auto &&evType : reqEvTypes)
        m_eventBus.Unsubscribe(std::dynamic_pointer_cast<EventReceiver>(pr) , evType);
}

void ProcessManager::ActivateProcess(std::shared_ptr<IProcess> pr)
{
    std::cout << "activate Proc: " << static_cast<int>(pr->GetPrId()) << std::endl;
    pr->SetFlag(IProcess::fActive);
    auto &&reqEvTypes = pr->GetIEventReceiver()->GetRequiredEventTypes();
    for (auto &&evType : reqEvTypes)
        m_eventBus.Subscribe(std::dynamic_pointer_cast<EventReceiver>(pr), evType);
}