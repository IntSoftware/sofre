
#ifndef SOFRE_MATH_HPP
#define SOFRE_MATH_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace sofre {

using vec3 = glm::vec3;
using mat4 = glm::mat4;


// vertex with positions
struct VertexP {
    float x, y, z;
};

// vertex with positions and colors
struct VertexPC {
    float x, y, z;
    float r, g, b;
};

// vertex with positions + normal + texCoord
struct VertexPNT {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

// vertex with positions + normal + colors + texCoord
struct VertexPNCT {
    float x, y, z;
    float nx, ny, nz;
    float r, g, b;
    float u, v;
};

}

#endif // SOFRE_MATH_HPP