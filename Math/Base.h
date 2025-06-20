#pragma once

class MVertex 
{
public:
    double x;
    double y;
    double z;

    MVertex()
        : x(0), y(0), z(0)
    {}
    MVertex(double x, double y, double z)
        : x(x), y(y), z(z)
    {}
};