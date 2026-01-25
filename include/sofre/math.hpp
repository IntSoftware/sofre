
#ifndef SOFRE_MATH_HPP
#define SOFRE_MATH_HPP

namespace sofre {

using real = float;

struct vec3 {
    real x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(real x, real y, real z) : x(x), y(y), z(z) {}
};

struct mat4 {
    real m[16]; // column-major
    mat4() { // hardcoded identity matrix
        m[0] = 1.0f; m[4] = 0.0f; m[8]  = 0.0f; m[12] = 0.0f;
        m[1] = 0.0f; m[5] = 1.0f; m[9]  = 0.0f; m[13] = 0.0f;
        m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
        m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
    }
};

struct VertexP   { real p[3]; };
struct VertexPN  { real p[3], n[3]; };
struct VertexPNU { real p[3], n[3], uv[2]; };

}

#endif // SOFRE_MATH_HPP