#ifndef SOFRE_SCENE_HPP
#define SOFRE_SCENE_HPP 

#include <sofre/object.hpp>

#include <cstdint>

namespace sofre {

class Scene {
public:
    using ObjectId = uint32_t;

    Scene();
    ~Scene();

    ObjectId createObject(const float* vertices, size_t size);
    void destroyObject(ObjectId id);

    Object& get(ObjectId id);
    const Object& get(ObjectId id) const;

    void drawObjects() const;

private:
    struct Objects;
    Objects* m_objects;
};

} // namespace sofre
#endif // SOFRE_SCENE_HPP