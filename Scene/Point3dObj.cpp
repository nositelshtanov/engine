#include "Point3dObj.h"

Vertex3dObj::Vertex3dObj()
    : Object3DBase(), m_point()
{
}

Vertex3dObj::Vertex3dObj(const MPoint3D &p)
    : Object3DBase(), m_point(p)
{
}

MPoint3D Vertex3dObj::GetPoint() const 
{ 
    return m_point; 
}

std::vector<MVertex3D> Vertex3dObj::GetVertexes() const
{
    return {MVertex3D(m_point)};
}

Vertex3dObj::~Vertex3dObj() = default;