#ifndef SOFRE_SCENE_HPP
#define SOFRE_SCENE_HPP 

#include <sofre/object.hpp>

#include <memory>
#include <list>
#include <algorithm>
#include <cstdint>
#include <limits>

namespace sofre {

class Scene {
    // TODO : better collection for Obejct, considering..
    // 1. memory area consistency(better cache hit)
    // 2. less memory allocation
    // 3. low overhead of add/remove
    using Objects = std::list<std::shared_ptr<Object>>;
public:
    using Handle = uint32_t;
    static constexpr Handle invalidHandle = std::numeric_limits<Handle>::max();

    Scene() = default;
    ~Scene() { destroy(); }

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addObject(const std::shared_ptr<Object>& obj) {
        m_objects.push_back(obj);
    }

    const Objects& objects() const {
        return m_objects;
    }

    void removeObject(const std::shared_ptr<Object>& obj) {
        auto it = std::find(
            m_objects.begin(),
            m_objects.end(),
            obj
        );
        if (it != m_objects.end())
            m_objects.erase(it);
    }


    bool active() const { return m_active; }
    bool valid() const { return m_handle != invalidHandle; }

    Handle handle() const { return m_handle; }

    void destroy() {
        m_objects.clear();
        m_active = false;
        m_handle = invalidHandle;
    }

private:
    friend class Renderer;

    void activate() {
        if (valid()) m_active = true;
    }
    void deactivate() { m_active = false; }

    void initialize(Handle handle) {
        m_handle = handle;
        m_active = true;
    }

    Objects m_objects;
    Handle m_handle = invalidHandle;
    bool m_active = true;
};

} // namespace sofre
#endif // SOFRE_SCENE_HPP