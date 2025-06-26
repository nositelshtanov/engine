#include "PrResult.h"

PrPointResult::PrPointResult(const MPoint3D& point)
        : IPrResult()
        , m_point(point)
{
}

PrResultType PrPointResult::GetType() const 
{ 
    return PrResultType::Point; 
}

MPoint3D PrPointResult::GetPoint() const 
{ 
    return m_point; 
}

PrPointResult::~PrPointResult() = default;


PrObjResult::PrObjResult()
    : IPrResult()
    , m_obj(nullptr)
{}

PrObjResult::PrObjResult(std::shared_ptr<IObject3D> obj)
    : IPrResult()
    , m_obj(std::move(obj))
{}

PrResultType PrObjResult::GetType() const 
{ 
    return PrResultType::Object; 
}

std::shared_ptr<IObject3D> PrObjResult::GetObj() const 
{ 
    return m_obj; 
}

PrObjResult::~PrObjResult() = default;