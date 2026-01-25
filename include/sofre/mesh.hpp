#ifndef SOFRE_MESH_HPP
#define SOFRE_MESH_HPP

#include <sofre/enums.hpp>

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>
#include <memory>

namespace sofre {

struct VertexAttributeDesc {
    uint32_t location;      // shader layout(location = X)
    uint32_t components;    // 1~4 (float, vec2, vec3, vec4 )
    VertexAttribType type;  // float, int, ...
    bool normalized;        // specifies whether fixed-point data values should be normalized
    size_t offset;        // byte offset in vertex
};

struct VertexLayout {
    size_t stride; // sizeof(Vertex)
    std::vector<VertexAttributeDesc> attributes;
};


class Mesh {
public:
    /*
    * Create a simple mesh with only position attribute (vec3 float)
    */
    static std::shared_ptr<Mesh> create(const float* positions, size_t size);
    static inline std::shared_ptr<Mesh> create(const void* data, size_t size,
                                               const VertexLayout& layout) {
        return std::shared_ptr<Mesh>(new Mesh(data, size, layout));
    }

    static std::shared_ptr<Mesh> loadOBJFile(const std::filesystem::path& file);
    static std::shared_ptr<Mesh> loadOBJString(const std::string& str);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    ~Mesh();

    int vertexCount() const { return m_count; }

    void draw() const;

private:
    Mesh(const void* data, size_t size, const VertexLayout& layout);

    struct Mesh_GL;
    Mesh_GL* gl;
    int m_count;
};

} // namespace sofre
#endif // SOFRE_MESH_HPP