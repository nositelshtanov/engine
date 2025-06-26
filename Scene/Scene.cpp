#include "Scene.h"

size_t Scene::s_lastUnusedId = 0;

Scene::Scene()
    : IScene()
    , m_id(s_lastUnusedId++)
    , m_objects()
{
}

void Scene::Init()
{
    CreateDefaultObjects();
}

size_t Scene::GetSceneId() const 
{ 
    return m_id; 
}

IScene::ObjectSPtr Scene::GetObjById(size_t id) const
{
    return m_objects.at(id);
}

std::vector<IScene::ObjectSPtr> Scene::GetAllObjs() const
{
    std::vector<ObjectSPtr> res;
    res.reserve(m_objects.size());
    for (auto &&[id, obj] : m_objects)
        res.push_back(obj);
    return res;
}

std::vector<std::shared_ptr<IDrawable>> Scene::GetAllDrawableObjs() const
{
    std::vector<std::shared_ptr<IDrawable>> res;
    for (auto &&[id, obj] : m_objects)
        if (obj->GetIDrawable())
            res.push_back(std::static_pointer_cast<IDrawable>(std::static_pointer_cast<Object3DBase>(obj)));

    return res;
}

void Scene::AddObject(ObjectSPtr obj)
{
    m_objects.insert({obj->GetObjId(), obj});
}

Scene::~Scene() = default;

void Scene::CreateDefaultObjects()
{
    // создать оси, базовые плоскости, origin
}