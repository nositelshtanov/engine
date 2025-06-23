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
    Scene()
        : IScene()
        , m_id(s_lastUnusedId++)
        , m_objects()
    {}

    void Init()
    {
        CreateDefaultObjects();
    }

    virtual size_t GetSceneId() const override { return m_id; }
    virtual IScene::ObjectSPtr GetObjById(size_t id) const 
    {
        return m_objects.at(id);
    }
    virtual std::vector<ObjectSPtr> GetAllObjs() const 
    {
        std::vector<ObjectSPtr> res;
        res.reserve(m_objects.size());
        for (auto && [id, obj] : m_objects)
            res.push_back(obj); 
        return res;
    }

    virtual std::vector<std::shared_ptr<IDrawable>> GetAllDrawableObjs() const 
    {
        std::vector<std::shared_ptr<IDrawable>> res;
        for (auto && [id, obj] : m_objects)
            if (obj->GetIDrawable())
                res.push_back(std::static_pointer_cast<IDrawable>(std::static_pointer_cast<Object3DBase>(obj)));

        return res;
    }

    virtual void AddObject(ObjectSPtr obj) 
    {
        m_objects.insert({obj->GetObjId(), obj});
    }

    virtual ~Scene() override = default;
private:
    void CreateDefaultObjects()
    {
        // создать оси, базовые плоскости, origin 
    }
};