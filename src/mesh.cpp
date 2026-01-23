#include <sofre/mesh.hpp>
#include <sofre/log.hpp>
#include "core.hpp"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust triangulation. Requires C++11
// and additional header
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

#include <fstream>
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
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
    void bind() const {
        glBindVertexArray(vao);
    }

    GLuint vao = 0;
    GLuint vbo = 0;
};

Mesh::Mesh(const float* data, size_t size, MeshAttribute attr, size_t stride)
    : m_attr(attr), m_stride(stride) {
    gl = new Mesh_GL();
   
    m_count = static_cast<int>(size / stride);

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    size_t offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          stride, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);

    if (hasMeshAttribute(attr, MeshAttribute::Normal)) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                              stride, (void*)offset);
        glEnableVertexAttribArray(1);
        offset += 3 * sizeof(float);
    }

    if (hasMeshAttribute(attr, MeshAttribute::UV)) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                              stride, (void*)offset);
        glEnableVertexAttribArray(2);
    }
}
static std::shared_ptr<Mesh> loadOBJ(tinyobj::ObjReader& reader,
                                     const std::string& errID) {
    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    if (attrib.vertices.empty() || shapes.empty()) {
        Log::error("OBJ file is empty : " + errID);
        return nullptr;
    }

    bool hasNormal = !attrib.normals.empty();
    bool hasUV = !attrib.texcoords.empty();

    MeshAttribute attr = MeshAttribute::Position;
    if (hasNormal)
        attr = attr | MeshAttribute::Normal;
    if (hasUV)
        attr = attr | MeshAttribute::UV;

    const size_t stride = 3 * sizeof(float) +
                          (hasNormal ? 3 * sizeof(float) : 0) +
                          (hasUV ? 2 * sizeof(float) : 0);

    std::vector<float> vertices;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {

            // position (always)
            int v = idx.vertex_index;
            vertices.push_back(attrib.vertices[3 * v + 0]);
            vertices.push_back(attrib.vertices[3 * v + 1]);
            vertices.push_back(attrib.vertices[3 * v + 2]);

            // normal (optional)
            if (hasNormal && idx.normal_index >= 0) {
                int n = idx.normal_index;
                vertices.push_back(attrib.normals[3 * n + 0]);
                vertices.push_back(attrib.normals[3 * n + 1]);
                vertices.push_back(attrib.normals[3 * n + 2]);
            }

            // uv (optional)
            if (hasUV && idx.texcoord_index >= 0) {
                int t = idx.texcoord_index;
                vertices.push_back(attrib.texcoords[2 * t + 0]);
                vertices.push_back(attrib.texcoords[2 * t + 1]);
            }
        }
    }

    if (vertices.empty()) {
        Log::error("Vertex data is empty : " + errID);
        return nullptr;
    }

    return Mesh::create(vertices.data(), vertices.size() * sizeof(float), attr,
                        stride);
}

std::shared_ptr<Mesh> Mesh::loadOBJFile(const std::filesystem::path& file) {
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;
    config.vertex_color = false;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(file.string(), config)) {
        Log::error("Failed to load OBJ file : " + file.string());
        Log::error("TinyOBJLoader error: " + reader.Error());
        return nullptr;
    }

    if (!reader.Warning().empty()) {
        Log::error("TinyOBJLoader warning: " + reader.Warning());
    }

    return loadOBJ(reader, file.string());
}

std::shared_ptr<Mesh> Mesh::loadOBJString(const std::string& str) {
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;
    config.vertex_color = false;
    const std::string partialstr = str.substr(0, 30) + "...";

    tinyobj::ObjReader reader;

    if (!reader.ParseFromString(str, "", config)) { //TODO : fix
        Log::error("Failed to read OBJ string : " + partialstr);
        Log::error("TinyOBJLoader error: " + reader.Error());
        return nullptr;
    }

    if (!reader.Warning().empty()) {
        Log::error("TinyOBJLoader warning: " + reader.Warning());
    }

    return loadOBJ(reader, partialstr);
}


Mesh::~Mesh() {
    delete gl;
}

void Mesh::draw() const {
    gl->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_count);
}

}
