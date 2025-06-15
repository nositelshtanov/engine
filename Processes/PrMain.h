#pragma once

#include "ProcessBase.h"

class PrMain : public ProcessBase
{
public:
    using BaseClass = ProcessBase;

    PrMain()
       : ProcessBase(static_cast<PrId>(PrIds::Main), nullptr) 
    {}

    virtual bool Run() override
    { 
        bool res = BaseClass::Run();        
        CreateChildProcesses();
        return res; 
    }

    virtual bool Stop() override
    { 
        bool res = BaseClass::Stop();
        return res;
    }

    virtual bool ReceiveEvent(const Event& event) override
    {
        auto et = event.GetEventType();
        switch (et)
        {
        case EventType::MouseEvent:
            /* code */
            break;
        case EventType::KeyBoardEvent:
            /* code */
            break;
        default:
            return false;
            break;
        }
    }

private:
    void CreateChildProcesses()
    {
        
    }
};