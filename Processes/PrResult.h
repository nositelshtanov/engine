#pragma once

#include "../Math/Base.h"

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