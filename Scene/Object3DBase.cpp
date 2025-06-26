#include "Object3DBase.h"

size_t Object3DBase::s_lastUnusedId = 0;

Object3DBase::Object3DBase()
    : m_id(s_lastUnusedId++)
{
}

size_t Object3DBase::GetObjId() const 
{ 
    return m_id; 
}

EventReceiver *Object3DBase::GetIEventReceiver()
{
    return this;
}

bool Object3DBase::ReceiveEvent(const Event &event)
{
    return false;
}

std::set<EventType> Object3DBase::GetRequiredEventTypes() const
{
    return {};
}

std::vector<MVertex3D> Object3DBase::GetVertexes() const 
{ 
    return {}; 
}

std::vector<MEdge3D> Object3DBase::GetEdges() const 
{ 
    return {}; 
}

std::vector<MFace3D> Object3DBase::GetFaces() const 
{ 
    return {}; 
}

IDrawable *Object3DBase::GetIDrawable() 
{ 
    return this; 
}

Object3DBase::~Object3DBase() = default;