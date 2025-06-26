#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "IScene.h"
#include "Object3DBase.h"

class Scene : public IScene
{
    size_t m_id;
    std::unordered_map<size_t, IScene::ObjectSPtr> m_objects;

    static size_t s_lastUnusedId;
public:
    Scene();

    void Init();

    virtual size_t GetSceneId() const override;
    virtual IScene::ObjectSPtr GetObjById(size_t id) const;
    virtual std::vector<ObjectSPtr> GetAllObjs() const;

    virtual std::vector<std::shared_ptr<IDrawable>> GetAllDrawableObjs() const;

    virtual void AddObject(ObjectSPtr obj);

    virtual ~Scene() override;
private:
    void CreateDefaultObjects();
};