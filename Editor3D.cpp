#include "Editor3D.h"

Editor3D::Editor3D(ProcessManager &prManager)
    : m_prManager(prManager)
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