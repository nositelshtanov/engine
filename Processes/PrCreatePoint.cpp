#include "PrCreatePoint.h"

PrCreatePoint::PrCreatePoint(PrIds id, IProcess * parent, Editor3D& editor)
    : ProcessBase(id, parent, editor)
    , m_pointObj(nullptr)
{}

bool PrCreatePoint::Run()
{
    bool res = BaseClass::Run();
    RunPickPointSubProcess();
    return res;
}

bool PrCreatePoint::Stop()
{
    /// TODO добавить объект в модель через контроллер
    return BaseClass::Stop();
}

bool PrCreatePoint::ReceiveEvent(const Event &event)
{
    return false;
}

bool PrCreatePoint::IsDone() const
{
    return m_pointObj.get();
}

bool PrCreatePoint::IsCancelled() const
{
    return !IsDone();
}

std::unique_ptr<IPrResult> PrCreatePoint::GetPrResult() const
{
    return std::make_unique<PrObjResult>(m_pointObj);
}

void PrCreatePoint::CancelCurState()
{
    Stop();
}

std::set<EventType> PrCreatePoint::GetRequiredEventTypes() const
{
    return {EventType::MouseEvent};
}

std::string PrCreatePoint::GetCurStateHint() const
{
    return "";
}

void PrCreatePoint::ChildStop(PrIds id)
{
    std::cout << "prCreateChildStop" << std::endl;
    if (id != PrIds::PickPoint)
        return;

    auto &&child = FindChildProc(id);
    if (!child)
        return;

    std::cout << "child is done" << std::endl;
    if (child->IsDone())
    {
        auto &&result = child->GetPrResult();
        if (result->GetType() == PrResultType::Point)
        {
            auto &&pointResult = static_cast<PrPointResult &>(*result);
            m_pointObj.reset(new Vertex3dObj(pointResult.GetPoint()));
            m_editor.GetCurScene().AddObject(m_pointObj);
            std::cout << "stop create point proc" << std::endl;
            Stop();
        }
    }
}

void PrCreatePoint::RunPickPointSubProcess()
{
    RunSubProc(PrIds::PickPoint);
}