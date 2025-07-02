#include "PrCreateEdge.h"

PrCreateEdge::PrCreateEdge(PrIds id, IProcess * parent, Editor3D& editor)
   : ProcessBase(id, parent, editor)
   , m_edgeObj(std::make_shared<Edge3dObj>())
   , m_twoPointsPicked(false)
{}

bool PrCreateEdge::Run() {
    bool res = BaseClass::Run();
    RunSubProc(PrIds::PickPoint);
    return res;
}

bool PrCreateEdge::Stop() {
    bool res = BaseClass::Stop();
    return res;
}

bool PrCreateEdge::ReceiveEvent(const Event& event) {
    return false;
}

bool PrCreateEdge::IsDone() const {
    return m_edgeObj && m_twoPointsPicked;
}

bool PrCreateEdge::IsCancelled() const {
    return !IsDone();
}

std::unique_ptr<IPrResult> PrCreateEdge::GetPrResult() const {
    return std::make_unique<PrObjResult>(m_edgeObj);
}

void PrCreateEdge::CancelCurState() {
    Stop();
}

std::set<EventType> PrCreateEdge::GetRequiredEventTypes() const {
    return {};
}

std::string PrCreateEdge::GetCurStateHint() const {
    return "";
}

void PrCreateEdge::ChildStop(PrIds id) {
    if (id != PrIds::PickPoint)
        return;

    auto &&child = FindChildProc(id);
    if (!child)
        return;

    if (child->IsDone()) {
        auto && result = child->GetPrResult();
        if (result->GetType() == PrResultType::Point) {
            auto && pointResult = static_cast<PrPointResult &>(*result).GetPoint();;
            if (!m_twoPointsPicked)
                m_edgeObj->SetBegVertex(pointResult);
            else {
                m_twoPointsPicked = true;
                m_edgeObj->SetEndVertex(pointResult);
                m_editor.GetCurScene().AddObject(m_edgeObj);
                Stop();
            }
        }
    }
}