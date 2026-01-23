#ifndef SOFRE_MESH_HPP
#define SOFRE_MESH_HPP

#include <sofre/enums.hpp>

#include <cstddef>
#include <filesystem>
#include <string>
#include <memory>

namespace sofre {

class Mesh {
public:
    static inline std::shared_ptr<Mesh> create(const float* vertices, size_t size) {
        return std::shared_ptr<Mesh>(new Mesh{vertices, size, MeshAttribute::Position, sizeof(float) * 3});
    }
    static inline std::shared_ptr<Mesh> create(const float* data, size_t size, MeshAttribute attr, size_t stride){
        return std::shared_ptr<Mesh>(new Mesh{data, size, attr, stride});
    }
    
    static std::shared_ptr<Mesh> loadOBJFile(const std::filesystem::path& file);
    static std::shared_ptr<Mesh> loadOBJString(const std::string& str);\

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    ~Mesh();

    MeshAttribute attributes() const { return m_attr; }
    int vertexCount() const { return m_count; }

    void draw() const;

private:
    Mesh(const float* data, size_t size, MeshAttribute attr, size_t stride);

    struct Mesh_GL;
    Mesh_GL* gl;
    MeshAttribute m_attr;
    int m_count;
    size_t m_stride;
};

} // namespace sofre
#endif // SOFRE_MESH_HPP