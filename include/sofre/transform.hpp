#ifndef SOFRE_TRANSFORM_HPP
#define SOFRE_TRANSFORM_HPP

#include <sofre/math.hpp>

namespace sofre {

struct Transform {
    Transform() = default;
    Transform(const Transform&) = delete;
    Transform& operator=(const Transform&) = delete;
    
    vec3 position {0,0,0};
    vec3 rotation {0,0,0};
    vec3 scale    {1,1,1};

    mat4 modelMatrix() const;
};

} // namespace sofre

#endif
