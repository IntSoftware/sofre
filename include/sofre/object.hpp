
#ifndef SOFRE_OBJECT_HPP
#define SOFRE_OBJECT_HPP

#include <cstddef>
#include <memory>

namespace sofre {

class Object {
public:
    Object(const float* vertices, size_t size);
    
    Object(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;

    ~Object();

    void draw() const;
    int vertexCount() const;
    
private:
    struct Object_GL;
    Object_GL* gl = nullptr;
    int m_count = 0;
};

}

#endif //SOFRE_OBJECT_HPP