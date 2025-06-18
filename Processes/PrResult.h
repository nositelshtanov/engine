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
    Point2d m_point;
public:
    PrPointResult(const Point2d& point)
        : IPrResult()
        , m_point(point)
    {}
    virtual PrResultType GetType() const override { return PrResultType::Point; }
    Point2d GetPoint() const { return m_point; }

    virtual ~PrPointResult() override = default;
};

class PrObjResult : public IPrResult
{
    std::shared_ptr<IObject3D> m_obj;
public:
    PrObjResult()
        : IPrResult()
        , m_obj(nullptr)
    {}
    PrObjResult(std::shared_ptr<IObject3D> obj)
        : IPrResult()
        , m_obj(std::move(obj))
    {}


    virtual PrResultType GetType() const override { return PrResultType::Object; }
    std::shared_ptr<IObject3D> GetObj() const { return m_obj; }

    virtual ~PrObjResult() = default;
};