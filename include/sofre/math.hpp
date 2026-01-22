
#ifndef SOFRE_MATH_HPP
#define SOFRE_MATH_HPP

namespace sofre {

struct vec3f {
    float x, y, z;
};

struct mat4f {
    float m[16]; // column-major
};

using vec3 = vec3f;
using mat4 = mat4f;

}

#endif // SOFRE_MATH_HPP