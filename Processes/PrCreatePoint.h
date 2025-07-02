#pragma once

#include <memory>

#include "PrResult.h"
#include "../Editor/Editor3D.h"
#include "ProcessCreator.h"
#include "ProcessBase.h"
#include "../Scene/Point3dObj.h"


class PrCreatePoint : public ProcessBase
{
    std::shared_ptr<Vertex3dObj> m_pointObj;
public:
    using BaseClass = ProcessBase;

    PrCreatePoint(PrIds id, IProcess * parent, Editor3D& editor);

    virtual bool Run();
    virtual bool Stop();
    virtual bool ReceiveEvent(const Event& event);
    virtual bool IsDone() const;
    virtual bool IsCancelled() const;
    virtual std::unique_ptr<IPrResult> GetPrResult() const;
    virtual void CancelCurState();
    virtual std::set<EventType> GetRequiredEventTypes() const;
    virtual std::string GetCurStateHint() const;
    virtual void ChildStop(PrIds id);

private:
    void RunPickPointSubProcess();
};
