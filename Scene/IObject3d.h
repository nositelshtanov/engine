#pragma once

class IObject3D
{
public:
    size_t GetObjId() const = 0;

    virtual ~IObject3D() = default;
};