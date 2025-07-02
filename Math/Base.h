#pragma once

// подумать как в организовать связи между мат. объектами и память

class MPoint3D
{
public:
    double x;
    double y;
    double z;

    MPoint3D()
        : x(0), y(0), z(0)
    {}
    MPoint3D(double x, double y, double z)
        : x(x), y(y), z(z)
    {}
};

class MVertex3D
{
    MPoint3D m_point;
public:
    MVertex3D()
        : m_point()
    {}
    MVertex3D(const MPoint3D& point)
        : m_point(point)
    {}
    MPoint3D GetPoint() const { return m_point; }
};

class MEdge3D
{
    MVertex3D m_begin;
    MVertex3D m_end;
public:
    MEdge3D()
        : m_begin()
        , m_end()
    {}
    void SetBegVertex(const MVertex3D& vertex) {
        m_begin = vertex;
    }
    void SetEndVertex(const MVertex3D& vertex) {
        m_end = vertex;
    }
};

class MFace3D
{
public:

};