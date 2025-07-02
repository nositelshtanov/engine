#pragma once

#include "../Editor/Editor3D.h"
#include "ProcessCreator.h"
#include "PrPickPoint.h"
#include "../EventBus/Event.h"


class PrMain : public ProcessBase
{
public:
    using BaseClass = ProcessBase;

    PrMain(Editor3D& editor);

    virtual bool Run() override;
    virtual bool Stop() override;
    virtual bool ReceiveEvent(const Event& event) override;
    virtual std::set<EventType> GetRequiredEventTypes() const;
    virtual std::unique_ptr<IPrResult> GetPrResult() const; 
    virtual bool IsDone() const;
    virtual bool IsCancelled() const; 
    virtual std::string GetCurStateHint() const;
    virtual void CancelCurState(); 

    virtual void ChildStop(PrIds id);

    virtual void Activated();
    virtual void Deactivated();
private:
    void RunSubProcess(PrIds id);
};