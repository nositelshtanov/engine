#include "Editor3D.h"

Editor3D::Editor3D(ProcessManager &prManager)
    : m_prManager(prManager)
    , m_defaultScene()
    , m_procHint(new ProcTextHint())
{
}

ProcessManager &Editor3D::GetPrManager() 
{ 
    return m_prManager; 
}

Scene &Editor3D::GetCurScene() 
{ 
    return m_defaultScene; 
}

std::shared_ptr<ProcTextHint> Editor3D::GetProcTextHint() const {
    return m_procHint;
}

void Editor3D::SetHint(const std::string& hint) {
    m_procHint->SetText(hint);
}

bool Editor3D::ReceiveEvent(const Event &event)
{
    return false;
}

std::set<EventType> Editor3D::GetRequiredEventTypes() const
{
    return {};
}

void Editor3D::FinishWork()
{
}

Editor3D::~Editor3D()
{
    FinishWork();
}