#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "IScene.h"

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

    virtual ~Scene() override = default;
private:
    void CreateDefaultObjects()
    {
        // создать оси, базовые плоскости, origin 
    }
};