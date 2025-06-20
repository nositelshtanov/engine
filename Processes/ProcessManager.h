#pragma once

#include <vector>

#include "IProcess.h"
#include "../EventBus/EventBus.h"

class ProcessManager 
{
    std::vector<IProcess*> m_stack;
    EventBus& m_eventBus;
public:
    ProcessManager(EventBus& eventBus)
        : m_eventBus(eventBus)
    {}

    void PushProcess(IProcess * pr)
    {
        DeactivateProcess(m_stack.back());
        m_stack.push_back(pr);
        ActivateProcess(pr); 
    }

    IProcess * PopProcess()
    {
        auto * pr = m_stack.back();
        DeactivateProcess(pr); 
        m_stack.pop_back();
        if (!m_stack.empty())
            ActivateProcess(m_stack.back());
        return pr;
    }

    void FinishWork()
    {

    }
private:
    void DeactivateProcess(IProcess * pr)
    {
        pr->UnsetFlag(IProcess::fActive);
        auto && reqEvTypes = pr->GetIEventReceiver()->GetRequiredEventTypes();
        for (auto && evType : reqEvTypes)
            m_eventBus.Unsubscribe(*pr->GetIEventReceiver(), evType);
    }
    void ActivateProcess(IProcess * pr)
    {
        pr->SetFlag(IProcess::fActive);
        auto && reqEvTypes = pr->GetIEventReceiver()->GetRequiredEventTypes();
        for (auto && evType : reqEvTypes)
            m_eventBus.Subscribe(*pr->GetIEventReceiver(), evType);
    }
};