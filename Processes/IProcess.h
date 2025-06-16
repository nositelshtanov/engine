#pragma once

#include <set>
#include <memory>
#include <string>

#include "../EventBus/Event.h"
#include "../EventBus/EventBus.h"
#include "PrResult.h"

using PrId = size_t;

enum class PrIds : PrId
{
    Main = 0,
    PickPoint,
    count
};


class IProcess : public EventReceiver
{
public:
    enum ProcessFlags
    {
        fActive = 1,
        fRunning = 2,
        fDone = 3,
        fCancelled = 4
    };
    
    virtual PrId GetPrId() const = 0;

    virtual bool Run() = 0;
    virtual bool Stop() = 0;

    virtual IProcess* GetParent() = 0;
    virtual std::set<std::shared_ptr<IProcess>> GetChilds() = 0;

    virtual size_t GetFlags() const = 0;
    virtual bool GetFlagVal(ProcessFlags flag) const = 0;
    virtual void SetFlag(IProcess::ProcessFlags flag) = 0; 
    virtual void UnsetFlag(ProcessFlags flag) = 0; 

    virtual std::unique_ptr<IPrResult> GetPrResult() const = 0;
    virtual bool IsDone() const = 0;
    virtual bool IsCancelled() const = 0;
    virtual std::string GetCurStateHint() const = 0;
    virtual void CancelCurState() = 0;

    virtual std::set<EventType> GetRequiredEventTypes() const = 0;

    virtual void ChildStop(PrIds id) = 0;

    virtual ~IProcess() override = default;
};