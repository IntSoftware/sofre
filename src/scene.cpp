#include <sofre/scene.hpp>
#include <sofre/object.hpp>
#include <sofre/log.hpp>

#include <map>
#include <memory>
#include <limits>

namespace sofre {

struct Scene::Objects {
    static inline constexpr ObjectId ID_MIN = std::numeric_limits<ObjectId>::min() + 1;
    
    std::map<ObjectId, std::unique_ptr<Object>> map;
    ObjectId nextId = ID_MIN;
};

Scene::Scene()
    : m_objects(new Objects)
{
}

Scene::~Scene()
{
    delete m_objects;
}

Scene::ObjectId Scene::createObject(const float* vertices, size_t size)
{
    ObjectId id;
    if(m_objects->nextId == std::numeric_limits<ObjectId>::max()) {
        ObjectId minFreeId = Objects::ID_MIN;
        for (const auto& [id, obj] : m_objects->map) {
            if (id == minFreeId) ++minFreeId;
            else break;
        }
        id = minFreeId;
        if (id == std::numeric_limits<ObjectId>::max()) {
            Log::error("[Scene::createObject] Exceeded maximum number of objects in Scene");
            Log::error("[Scene::createObject] Current object count : " + std::to_string(m_objects->map.size()));
            return std::numeric_limits<ObjectId>::min();
        }
    } else {
        id = m_objects->nextId++;
    }
    m_objects->map.try_emplace(
        id,
        std::make_unique<Object>(vertices, size)
    );
    return id;
}

void Scene::destroyObject(ObjectId id)
{
    m_objects->map.erase(id);
}

Object& Scene::get(ObjectId id)
{
    auto it = m_objects->map.find(id);
    if (it == m_objects->map.end())
        Log::error("[Scene::get] Invalid ObjectId : " + std::to_string(id));
    return *it->second;
}

const Object& Scene::get(ObjectId id) const
{
    auto it = m_objects->map.find(id);
    if (it == m_objects->map.end())
        Log::error("[Scene::get] Invalid ObjectId : " + std::to_string(id));
    return *it->second;
}

void Scene::drawObjects() const {
    for (const auto& [id, obj] : m_objects->map)
        obj->draw();
}

} // namespace sofre
