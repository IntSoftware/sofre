
#ifndef SOFRE_OBJECT_HPP
#define SOFRE_OBJECT_HPP

#include <sofre/mesh.hpp>
#include <sofre/transform.hpp>
#include <sofre/shader.hpp>
#include <sofre/program.hpp>

#include <cstddef>
#include <memory>
#include <unordered_map>

namespace sofre {

class Object {
public:
    static inline std::shared_ptr<Object> create(std::shared_ptr<Mesh> mesh) {
        return std::shared_ptr<Object>(new Object(mesh));
    }
    template<typename... Args>
    static inline std::shared_ptr<Object> create(Args&&... args) {
        return create(Mesh::create(std::forward<Args>(args)...));
    }   
    Object(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    ~Object() = default;

    Transform& transform() { return m_transform; }
    const Transform& transform() const { return m_transform; }

    const Mesh& mesh() const { return *m_mesh; }

    void setUniform(std::string name, const Uniform& value) {
        m_uniforms[name] = value;
    }
    void applyUniforms(const Program& program) const {
        for (const auto& [name, value] : m_uniforms) {
            program.setUniform(name.c_str(), value);
    }
}

private:
    explicit Object(std::shared_ptr<Mesh> mesh)
        : m_mesh(std::move(mesh)) {}

    std::unordered_map<std::string, Uniform> m_uniforms;
    std::shared_ptr<Mesh> m_mesh;
    Transform m_transform;
};

}

#endif //SOFRE_OBJECT_HPP