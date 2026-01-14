
#ifndef SOFRE_OBJECT_HPP
#define SOFRE_OBJECT_HPP

#include <cstddef>
#include <memory>

namespace sofre {

class Object {
public:
    ~Object();

    void bind() const;
    int vertexCount() const;
    
    static std::shared_ptr<Object> generate(const float* vertices, size_t size);
private:
    Object(const float* vertices, size_t size);
    
    struct Object_GL;
    Object_GL* gl = nullptr;
    int m_count = 0;
};

}

#endif //SOFRE_OBJECT_HPP