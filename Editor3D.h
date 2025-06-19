#pragma once

#include "Processes/ProcessManager.h"
#include "Scene/Scene.h"

class Editor3D : public EventReceiver
{
    ProcessManager& m_prManager;
    Scene m_defaultScene;
public:
    Editor3D(ProcessManager& prManager)
        : m_prManager(prManager)
    {}

    ProcessManager& GetPrManager() { return m_prManager; }
    Scene& GetCurScene() { return m_defaultScene; }

    virtual bool ReceiveEvent(const Event& event) 
    {
        return false;
    }

    virtual std::set<EventType> GetRequiredEventTypes() const 
    {
        return {};
    }
};