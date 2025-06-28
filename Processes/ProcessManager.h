#pragma once

#include <vector>
#include <memory>

#include "IProcess.h"
#include "../EventBus/EventReceiver.h"
#include "../EventBus/EventBus.h"

class ProcessManager 
{
    std::vector<std::shared_ptr<IProcess>> m_stack;
    EventBus& m_eventBus;
public:
    ProcessManager(EventBus& eventBus);

    void PushProcess(std::shared_ptr<IProcess> pr);

    std::shared_ptr<IProcess> PopProcess();
    void FinishWork();
private:
    void DeactivateProcess(std::shared_ptr<IProcess> pr);
    void ActivateProcess(std::shared_ptr<IProcess> pr);
};