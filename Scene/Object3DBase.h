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
    Object3DBase()
        : m_id(s_lastUnusedId++)
    {}
    size_t GetObjId() const { return m_id; } 
    virtual EventReceiver* GetIEventReceiver() override 
    {
        return this;
    }

    virtual bool ReceiveEvent(const Event& event) override
    {
        return false;
    }
    virtual std::set<EventType> GetRequiredEventTypes() const override
    {
        return {};
    }

    virtual std::vector<MVertex3D> GetVertexes() const override { return {}; }
    virtual std::vector<MEdge3D> GetEdges() const override { return {}; }
    virtual std::vector<MFace3D> GetFaces() const override { return {}; }

    virtual IDrawable* GetIDrawable() override { return this; }

    virtual ~Object3DBase() override = default;
};