#pragma once

#include <memory>

#include "../Processes/ProcessManager.h"
#include "../Scene/Scene.h"
#include "../Processes/ProcTextHint.h"

class Editor3D : public EventReceiver
{
    ProcessManager& m_prManager;
    Scene m_defaultScene;
    std::shared_ptr<ProcTextHint> m_procHint;
public:
    Editor3D(ProcessManager& prManager);

    ProcessManager& GetPrManager();
    Scene& GetCurScene();

    std::shared_ptr<ProcTextHint> GetProcTextHint() const;
    void SetHint(const std::string& hint);

    virtual bool ReceiveEvent(const Event& event);
    virtual std::set<EventType> GetRequiredEventTypes() const;

    void FinishWork();

    virtual ~Editor3D() override;
};