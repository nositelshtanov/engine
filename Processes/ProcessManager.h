#pragma once

#include <vector>

#include "IProcess.h"
#include "../EventBus/EventBus.h"

class ProcessManager 
{
    std::vector<IProcess*> m_stack;
    EventBus& m_eventBus;
public:
    ProcessManager(EventBus& eventBus);

    void PushProcess(IProcess * pr);

    IProcess * PopProcess();
    void FinishWork();
private:
    void DeactivateProcess(IProcess * pr);
    void ActivateProcess(IProcess * pr);
};