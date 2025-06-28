#pragma once

#include <optional>

#include "../Editor3D.h"
#include "ProcessBase.h"
#include "../Math/Base.h"

class PrPickPoint : public ProcessBase
{
    std::optional<MPoint3D> m_point; 
public:
    using BaseClass = ProcessBase;

    PrPickPoint(PrIds id, IProcess * parent, Editor3D& editor);

    virtual bool Run();
    virtual bool Stop();

    virtual bool ReceiveEvent(const Event& event);
    virtual std::unique_ptr<IPrResult> GetPrResult() const;
    virtual bool IsDone() const;
    virtual bool IsCancelled() const;
    virtual std::string GetCurStateHint() const;
    virtual void CancelCurState();
    virtual std::set<EventType> GetRequiredEventTypes() const;
};
