#pragma once

#include <memory>

#include "PrResult.h"
#include "../Editor/Editor3D.h"
#include "ProcessCreator.h"
#include "ProcessBase.h"
#include "../Scene/Edge3dObj.h"

class PrCreateEdge : public ProcessBase {
    std::shared_ptr<Edge3dObj> m_edgeObj;
    bool m_onePointPicked;
public:
    using BaseClass = ProcessBase;

    PrCreateEdge(PrIds id, IProcess * parent, Editor3D& editor);

    virtual bool Run() override;
    virtual bool Stop() override;
    virtual bool ReceiveEvent(const Event& event) override;
    virtual bool IsDone() const override;
    virtual bool IsCancelled() const override;
    virtual std::unique_ptr<IPrResult> GetPrResult() const override;
    virtual void CancelCurState() override;
    virtual std::set<EventType> GetRequiredEventTypes() const override;
    virtual std::string GetCurStateHint() const override;
    virtual void ChildStop(PrIds id) override;

private:
};