#pragma once

#include "Object3DBase.h"
#include "../Math/Base.h"

class Vertex3dObj: public Object3DBase
{
    MPoint3D m_point; // пока что 2д
public:
    Vertex3dObj();
    Vertex3dObj(const MPoint3D& p);

    MPoint3D GetPoint() const;

    std::vector<MVertex3D> GetVertexes() const override;

    virtual ~Vertex3dObj();
};