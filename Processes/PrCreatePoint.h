#pragma once

#include <memory>

#include "PrResult.h"
#include "../Editor3D.h"
#include "ProcessCreator.h"
#include "ProcessBase.h"
#include "../Scene/Point3dObj.h"


class PrCreatePoint : public ProcessBase
{
    std::shared_ptr<Point3dObj> m_pointObj;
public:
    using BaseClass = ProcessBase;

    PrCreatePoint(PrIds id, IProcess * parent, Editor3D& editor)
        : ProcessBase(id, parent, editor)
        , m_pointObj(nullptr)
    {}

    virtual bool Run()
    {
        bool res = BaseClass::Run();
        RunPickPointSubProcess();
        return res;
    }
    
    virtual bool Stop()
    {
        for (auto && child : m_childs)
            child->Stop();
        /// TODO добавить объект в модель через контроллер
        return BaseClass::Stop();
    }

    virtual bool ReceiveEvent(const Event& event) 
    {
        return false;
    }

    virtual bool IsDone() const 
    {
        return m_pointObj.get();
    }

    virtual bool IsCancelled() const 
    {
        return !IsDone(); 
    }

    virtual std::unique_ptr<IPrResult> GetPrResult() const 
    {
        return std::make_unique<PrObjResult>(m_pointObj);
    }

    virtual void CancelCurState() 
    {
        Stop();
    }

    virtual std::set<EventType> GetRequiredEventTypes() const
    {
        return {EventType::MouseEvent};
    }
    
    virtual std::string GetCurStateHint() const 
    {
        return "";
    }

    virtual void ChildStop(PrIds id) 
    {
        if (id != PrIds::PickPoint)
            return;
        
        auto && child = FindChildProc(id);
        if (!child)
            return;
        if (child->IsDone())
        {
            auto && result = child->GetPrResult();
            if (result->GetType() == PrResultType::Point)
            {
                auto && pointResult = static_cast<PrPointResult&>(*result);
                m_pointObj.reset(new Point3dObj(pointResult.GetPoint()));
            }
        }
        m_childs.erase(child);
    }

private:
    void RunPickPointSubProcess()
    {
        RunSubProc(PrIds::PickPoint); 
    }
};
