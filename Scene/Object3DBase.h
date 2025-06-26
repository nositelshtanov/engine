#pragma once

#include "IObject3d.h"
#include "../EventBus/EventReceiver.h"
#include "../render/IDrawable.h"
#include "ObjectsConnector.h"

class Object3DBase : public IObject3D
                   , public EventReceiver
                   , public IDrawable
{
    size_t m_id;

    static size_t s_lastUnusedId;
public:
    Object3DBase();

    size_t GetObjId() const; 
    virtual EventReceiver* GetIEventReceiver() override;
    virtual bool ReceiveEvent(const Event& event) override;
    virtual std::set<EventType> GetRequiredEventTypes() const override;

    virtual std::vector<MVertex3D> GetVertexes() const override; 
    virtual std::vector<MEdge3D> GetEdges() const override;
    virtual std::vector<MFace3D> GetFaces() const override;

    virtual IDrawable* GetIDrawable() override; 

    virtual ~Object3DBase() override;
};