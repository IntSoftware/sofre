#ifndef SOFRE_MESH_HPP
#define SOFRE_MESH_HPP

#include <cstddef>
#include <memory>

namespace sofre {

class Mesh {
public:
    static inline std::shared_ptr<Mesh> create(const float* vertices, size_t size) {
        return std::shared_ptr<Mesh>(new Mesh{vertices, size});
    }
    // TODO : file
    //static inline std::shared_ptr<Mesh> create(const std::initializer_list<float>& vertices) {
    //    return std::make_shared<Mesh>(vertices.begin(), vertices.size() * sizeof(float));
    //}

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    ~Mesh();

    void draw() const;
    int vertexCount() const { return m_count; }
private:
    Mesh(const float* vertices, size_t size);
    struct Mesh_GL;
    Mesh_GL* gl;
    int m_count;
};

} // namespace sofre
#endif // SOFRE_MESH_HPP