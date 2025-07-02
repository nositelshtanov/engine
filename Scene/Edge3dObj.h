#pragma once

#include "Object3DBase.h"
#include "../Math/Base.h"

class Edge3dObj : public Object3DBase{
    MEdge3D m_edge;
public:
    Edge3dObj()
        : m_edge()
    {}

    void SetBegVertex(const MVertex3D& vertex);
    void SetEndVertex(const MVertex3D& vertex);

    virtual std::vector<MEdge3D> GetEdges() const override;

    virtual ~Edge3dObj() override = default;
};