#pragma once

#include <vector>

#include "../Math/Base.h"

class IDrawable 
{
public:
    virtual std::vector<MVertex3D> GetVertexes() const = 0; 
    virtual std::vector<MEdge3D> GetEdges() const = 0;
    virtual std::vector<MFace3D> GetFaces() const = 0;

    // добавить метод ручной отрисовки 
    // virtual void GlDraw() = 0;

    virtual ~IDrawable() = default;
};