// transform.cpp
#include <sofre/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sofre {

mat4 Transform::modelMatrix() const {
    glm::mat4 m(1.0f);

    m = glm::translate(m, {position.x, position.y, position.z});
    m = glm::rotate(m, rotation.x, {1,0,0});
    m = glm::rotate(m, rotation.y, {0,1,0});
    m = glm::rotate(m, rotation.z, {0,0,1});
    m = glm::scale(m, {scale.x, scale.y, scale.z});

    mat4 out;
    auto mptr = glm::value_ptr(m);
    //std::memcpy(out.m, glm::value_ptr(m), sizeof(float) * 16);
    for(int i = 0; i < 16; ++i)
        out.m[i] = mptr[i];
        
    return out;
}

}
