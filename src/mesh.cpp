#include <sofre/mesh.hpp>
#include <sofre/log.hpp>

#include <glad/gl.h>
#include <glutil/model.hpp>

#include <fstream>
#include <filesystem>
#include <cstddef>
#include <vector>

namespace sofre {

struct Mesh::Mesh_GL {
    Mesh_GL() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }
    ~Mesh_GL() {
        destroy();
    }
    void destroy() {
        if (ebo) glDeleteBuffers(1, &ebo);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (vao) glDeleteVertexArrays(1, &vao);
        vbo = vao = ebo = 0;
    }

    void bind() const {
        glBindVertexArray(vao);
    }

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
};

Mesh::Mesh(const void* data, size_t size, const VertexLayout& layout) {
    gl = new Mesh_GL();
    m_count = static_cast<int>(size / layout.stride);

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    for (const auto& attr : layout.attributes) {
        if (attr.type == VertexAttribType::Float) {
            glVertexAttribPointer(
                attr.location,
                attr.components,
                GL_FLOAT,
                attr.normalized,
                layout.stride,
                reinterpret_cast<void*>(attr.offset)
            );
        } else if (attr.type == VertexAttribType::Int) {
            glVertexAttribIPointer(
                attr.location,
                attr.components,
                GL_INT,
                layout.stride,
                reinterpret_cast<void*>(attr.offset)
            );
        } else {
            Log::err() << "Unsupported vertex attribute type : " << static_cast<int>(attr.type);
        }
        glEnableVertexAttribArray(attr.location);
    }
}

std::shared_ptr<Mesh> Mesh::create(const float* positions, size_t size) {
    VertexLayout layout;
    layout.stride = 3 * sizeof(float);
    layout.attributes = {{0, 3, VertexAttribType::Float, false, 0}};
    return std::shared_ptr<Mesh>(new Mesh(positions, size, layout));
}

std::shared_ptr<Mesh> Mesh::loadOBJFile(const std::filesystem::path& file) {
    glutil::GLModelData loaded = glutil::ModelLoader::loadOBJtoGL(file, true);

    // Surface loader warnings (e.g. missing materials, non-fatal parse issues).
    if (!loaded.warn.empty()) {
        Log::warn() << "OBJ load warning (" << file.string() << "): " << loaded.warn;
    }

    if (!loaded.ok || loaded.meshes.empty()) {
        Log::err() << "Failed to load OBJ file: " << file.string();
        if (!loaded.error.empty()) {
            Log::err() << "glutil::ModelLoader error: " << loaded.error;
        }
        return nullptr;
    }

    auto& gpu = loaded.meshes.front();
    auto result = std::shared_ptr<Mesh>(new Mesh());
    result->gl = new Mesh_GL();
    result->gl->vao = gpu.vao;
    result->gl->vbo = gpu.vbo;
    result->gl->ebo = gpu.ebo;
    result->m_count = static_cast<int>(gpu.indexCount);

    gpu.vao = 0;
    gpu.vbo = 0;
    gpu.ebo = 0;
    gpu.indexCount = 0;
    return result;
}

std::shared_ptr<Mesh> Mesh::loadOBJString(const std::string& str) {
    const auto tempPath = std::filesystem::temp_directory_path() /
        ("sofre_obj_" + std::to_string(std::hash<std::string>{}(str)) + ".obj");

    std::ofstream out(tempPath, std::ios::binary);
    if (!out) {
        Log::err() << "Failed to create temporary OBJ file for string load.";
        return nullptr;
    }
    out.write(str.data(), static_cast<std::streamsize>(str.size()));
    out.close();

    auto result = loadOBJFile(tempPath);
    std::error_code ec;
    std::filesystem::remove(tempPath, ec);
    return result;
}

Mesh::~Mesh() {
    delete gl;
}

void Mesh::draw() const {
    if (!gl) return;
    gl->bind();
    if (gl->ebo) {
        glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, m_count);
    }
}

void Mesh::destroy() {
    if (gl) {
        gl->destroy();
    }
}
} // namespace sofre
