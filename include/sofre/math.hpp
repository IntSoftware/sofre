
#ifndef SOFRE_MATH_HPP
#define SOFRE_MATH_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace sofre {

using vec3 = glm::vec3;
using mat4 = glm::mat4;

struct VertexP   { float p[3]; };
struct VertexPN  { float p[3], n[3]; };
struct VertexPNU { float p[3], n[3], uv[2]; };

}

#endif // SOFRE_MATH_HPP