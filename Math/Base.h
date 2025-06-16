#pragma once

class Point2d
{
public:
    double x;
    double y;

    Point2d()
        : x(0), y(0)
    {}
    Point2d(double x, double y)
        : x(x), y(y)
    {}
};