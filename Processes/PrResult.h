#pragma once

#include <memory>

#include "../Math/Base.h"
#include "../Scene/IObject3d.h"

enum class PrResultType
{
    Point,
    Object,
    count
};

class IPrResult 
{
public:
    virtual PrResultType GetType() const = 0;
    virtual ~IPrResult() = default;
};

class PrPointResult : public IPrResult
{
    MPoint3D m_point;
public:
    PrPointResult(const MPoint3D& point);

    virtual PrResultType GetType() const override;
    MPoint3D GetPoint() const;

    virtual ~PrPointResult() override;
};

class PrObjResult : public IPrResult
{
    std::shared_ptr<IObject3D> m_obj;
public:
    PrObjResult();
    PrObjResult(std::shared_ptr<IObject3D> obj);

    virtual PrResultType GetType() const override;
    std::shared_ptr<IObject3D> GetObj() const;

    virtual ~PrObjResult();
};