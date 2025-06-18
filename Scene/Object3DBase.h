#pragma once

#include "IObject3d.h"

class Object3DBase : public IObject3D
{
    size_t m_id;

    static size_t s_lastUnusedId;
public:
    Object3DBase()
        : m_id(s_lastUnusedId++)
    {}
    size_t GetObjId() const { return m_id; } 

    virtual ~Object3DBase() override = default;
};