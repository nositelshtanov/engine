#pragma once

#include <stddef.h>

class EventReceiver;

class IObject3D
{
public:
    virtual size_t GetObjId() const = 0;

    virtual EventReceiver* GetIEventReceiver() = 0;

    virtual ~IObject3D() = default;
};