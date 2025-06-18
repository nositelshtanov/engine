#pragma once

#include "Object3DBase.h"
#include "../Math/Base.h"

class Point3dObj : public Object3DBase
{
    Point2d m_point; // пока что 2д
public:
    Point3dObj()
        : Object3DBase()
        , m_point()
    {}

    Point3dObj(const Point2d& p)
        : Object3DBase()
        , m_point(p)
    {}

    Point2d GetPoint() const { return m_point; }

    virtual ~Point3dObj() = default;
};