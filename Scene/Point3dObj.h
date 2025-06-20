#pragma once

#include "Object3DBase.h"
#include "../Math/Base.h"

class Vertex3dObj: public Object3DBase
{
    MVertex m_vertex; // пока что 2д
public:
    Vertex3dObj()
        : Object3DBase()
        , m_vertex()
    {}

    Vertex3dObj(const MVertex& p)
        : Object3DBase()
        , m_vertex(p)
    {}

    MVertex GetPoint() const { return m_vertex; }

    virtual ~Vertex3dObj() = default;
};