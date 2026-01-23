
#ifndef SOFRE_MATH_HPP
#define SOFRE_MATH_HPP

namespace sofre {

using real = float;

struct vec3 {
    real x, y, z;
};

struct mat4 {
    real m[16]; // column-major
};

struct VertexP   { real p[3]; };
struct VertexPN  { real p[3], n[3]; };
struct VertexPNU { real p[3], n[3], uv[2]; };

}

#endif // SOFRE_MATH_HPP