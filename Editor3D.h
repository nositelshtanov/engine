#pragma once

#include "Processes/ProcessManager.h"
#include "Scene/Scene.h"

class Editor3D : public EventReceiver
{
    ProcessManager& m_prManager;
    Scene m_defaultScene;
public:
    Editor3D(ProcessManager& prManager);

    ProcessManager& GetPrManager();
    Scene& GetCurScene();

    virtual bool ReceiveEvent(const Event& event);
    virtual std::set<EventType> GetRequiredEventTypes() const;

    void FinishWork();

    virtual ~Editor3D() override;
};