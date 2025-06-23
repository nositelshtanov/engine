#pragma once

#include "../Scene/IScene.h"

class IRenderer
{
public:
    void AddScene(IScene* scene);

    virtual ~IRenderer() = default;
};