#pragma once

#include "Object3DBase.h"
#include "../Math/Base.h"

class Vertex3dObj: public Object3DBase
{
    MPoint3D m_point; // пока что 2д
public:
    Vertex3dObj()
        : Object3DBase()
        , m_point()
    {}

    Vertex3dObj(const MPoint3D& p)
        : Object3DBase()
        , m_point(p)
    {}

    MPoint3D GetPoint() const { return m_point; }

    std::vector<MVertex3D> GetVertexes() const override 
    {
        return {MVertex3D(m_point)};
    }

    virtual ~Vertex3dObj() = default;
};