
#ifndef SOFRE_OBJECT_HPP
#define SOFRE_OBJECT_HPP

#include <sofre/mesh.hpp>
#include <sofre/transform.hpp>
#include <sofre/program.hpp>
#include <sofre/log.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include <limits>

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

    
    using UniformCallback = std::function<void(const Program::UniformSetter&)>;
    using UniformHandle = uint32_t;

    UniformHandle addUniformCallback(UniformCallback cb) {
        // check next handle unused
        if (m_uniformCallbacks.find(m_nextUniformHandle) == m_uniformCallbacks.end()) {
            UniformHandle h = m_nextUniformHandle;
            m_uniformCallbacks.emplace(h, std::move(cb));
            // advance next handle if possible
            if (m_nextUniformHandle != std::numeric_limits<UniformHandle>::max())
                ++m_nextUniformHandle;
            return h;
        }

        // slow path: handle space exhausted or fragmented, find smallest free handle
        for (UniformHandle h = 1; h < std::numeric_limits<UniformHandle>::max(); ++h) {
            if (m_uniformCallbacks.find(h) == m_uniformCallbacks.end()) {
                m_uniformCallbacks.emplace(h, std::move(cb));
                return h;
            }
        }

        Log::error("UniformHandle exhausted: no free handle available");
        return 0;
    }
    void removeUniformCallback(UniformHandle h) {
        auto it = m_uniformCallbacks.find(h);
        if (it == m_uniformCallbacks.end())
            return;

        m_uniformCallbacks.erase(it);

        // if the removed handle was the most recently issued one,
        // roll back m_nextUniformHandle to keep it as small as possible
        if (h + 1 == m_nextUniformHandle) {
            while (m_nextUniformHandle > 1) {
                UniformHandle prev = m_nextUniformHandle - 1;
                if (m_uniformCallbacks.find(prev) != m_uniformCallbacks.end())
                    break;
                m_nextUniformHandle = prev;
            }
        }
    }

    void applyUniforms(const Program::UniformSetter& u) const {
        for (const auto& [_, cb] : m_uniformCallbacks) {
            cb(u);
        }
    }

    UniformHandle useModelMatrixUniform(const char* name) {
        return addUniformCallback(
            [this, uname = std::string(name)]
            (const Program::UniformSetter& u) {
                u.mat4(
                    uname.c_str(),
                    this->transform().modelMatrix()
                );
            }
        );
    }
private:
    explicit Object(std::shared_ptr<Mesh> mesh)
        : m_mesh(std::move(mesh)) {}

    std::shared_ptr<Mesh> m_mesh;
    Transform m_transform;

    std::unordered_map<UniformHandle, UniformCallback> m_uniformCallbacks;
    UniformHandle m_nextUniformHandle = 1;
};

}

#endif //SOFRE_OBJECT_HPP