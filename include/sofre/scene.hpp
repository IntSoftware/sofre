#ifndef SOFRE_SCENE_HPP
#define SOFRE_SCENE_HPP 

#include <sofre/object.hpp>

#include <memory>
#include <list>

namespace sofre {

class Scene {
    using Objects = std::list<std::shared_ptr<Object>>;
public:
    void addObject(const std::shared_ptr<Object>& obj) {
        m_objects.push_back(obj);
    }

    const Objects& objects() const {
        return m_objects;
    }

    void removeObject(std::shared_ptr<Object>& obj) {
        auto it = std::find(
            m_objects.begin(),
            m_objects.end(),
            obj
        );;
        if (it != m_objects.end())
            m_objects.erase(it);
    }

private:
    Objects m_objects;
};

} // namespace sofre
#endif // SOFRE_SCENE_HPP