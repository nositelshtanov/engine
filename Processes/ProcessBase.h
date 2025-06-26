#pragma once

#include <set>
#include <memory>
#include <iostream>

#include "IProcess.h"
#include "ProcessCreator.h"
#include "../Editor3D.h"

class ProcessBase : public EventReceiver 
                  , public IProcess
{
    PrIds m_id;
protected:
    IProcess* m_parent;
    size_t m_flags;
    std::set<std::shared_ptr<IProcess>> m_childs;
    Editor3D& m_editor;
public:
    ProcessBase(PrIds id, IProcess * parent, Editor3D& editor);
    virtual PrIds GetPrId() const override; 
    
    virtual bool Run() override;
    virtual bool Stop() override;
    virtual IProcess* GetParent() override; 
    virtual std::set<std::shared_ptr<IProcess>> GetChilds() override; 

    virtual bool ReceiveEvent(const Event& event) override;

    virtual size_t GetFlags() const override; 
    virtual void SetFlag(IProcess::ProcessFlags flag);
    virtual void UnsetFlag(ProcessFlags flag);
    virtual bool GetFlagVal(ProcessFlags flag) const override;

    virtual std::unique_ptr<IPrResult> GetPrResult() const;
    virtual bool IsDone() const;
    virtual bool IsCancelled() const;

    virtual std::set<EventType> GetRequiredEventTypes() const;

    virtual void ChildStop(PrIds id);

    virtual EventReceiver * GetIEventReceiver();

    std::shared_ptr<IProcess> RunSubProc(PrIds id);
    std::shared_ptr<IProcess> FindChildProc(PrIds id);

    virtual ~ProcessBase() override;
};