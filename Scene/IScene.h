#pragma once

#include <stddef.h>
#include "IObject3d.h"

class IScene
{
public:
    using ObjectSPtr = std::shared_ptr<IObject3D>;
    IScene()
    {}

    virtual size_t GetSceneId() const = 0;
    virtual ObjectSPtr GetObjById(size_t id) const = 0;
    virtual std::vector<ObjectSPtr> GetAllObjs() const = 0;
    virtual std::vector<std::shared_ptr<IDrawable>> GetAllDrawableObjs() const = 0;

    virtual void AddObject(ObjectSPtr obj) = 0;

    virtual ~IScene() = default;
};