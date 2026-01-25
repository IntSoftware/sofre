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
                (void*)attr.offset
            );
        } else if(attr.type == VertexAttribType::Int){
            glVertexAttribIPointer(
                attr.location,
                attr.components,
                GL_INT,
                layout.stride,
                (void*)attr.offset
            );
        } else {
            Log::error("Unsupported vertex attribute type : " + std::to_string((int)attr.type));
        }

        glEnableVertexAttribArray(attr.location);
    }
}

std::shared_ptr<Mesh> Mesh::create(const float* positions, size_t size) {
    VertexLayout layout;
    layout.stride = 3 * sizeof(float);
    layout.attributes = {
        {
            0, // location
            3, // vec3
            VertexAttribType::Float,
            false,
            0
        }
    };

    return std::shared_ptr<Mesh>(new Mesh(positions, size, layout));
}


static std::shared_ptr<Mesh> loadOBJ(tinyobj::ObjReader& reader,
                                     const std::string& errID) {
    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    if (attrib.vertices.empty() || shapes.empty()) {
        Log::error("OBJ file is empty : " + errID);
        return nullptr;
    }

    const bool hasNormal = !attrib.normals.empty();
    const bool hasUV     = !attrib.texcoords.empty();

    VertexLayout layout;
    size_t offset = 0;

    // position (location = 0)
    layout.attributes.push_back({
        0,                          // location
        3,                          // vec3
        VertexAttribType::Float,
        false,
        offset
    });
    offset += 3 * sizeof(float);

    // normal (location = 1)
    if (hasNormal) {
        layout.attributes.push_back({
            1,
            3,
            VertexAttribType::Float,
            false,
            offset
        });
        offset += 3 * sizeof(float);
    }

    // uv (location = 2)
    if (hasUV) {
        layout.attributes.push_back({
            2,
            2,
            VertexAttribType::Float,
            false,
            offset
        });
        offset += 2 * sizeof(float);
    }
    layout.stride = offset;

    std::vector<float> vertices;
    vertices.reserve(shapes.size() * 3 * layout.stride / sizeof(float));

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {

            // position
            int v = idx.vertex_index;
            vertices.push_back(attrib.vertices[3 * v + 0]);
            vertices.push_back(attrib.vertices[3 * v + 1]);
            vertices.push_back(attrib.vertices[3 * v + 2]);

            // normal
            if (hasNormal) {
                if (idx.normal_index >= 0) {
                    int n = idx.normal_index;
                    vertices.push_back(attrib.normals[3 * n + 0]);
                    vertices.push_back(attrib.normals[3 * n + 1]);
                    vertices.push_back(attrib.normals[3 * n + 2]);
                } else {
                    // fallback
                    vertices.insert(vertices.end(), {0.f, 0.f, 0.f});
                }
            }

            // uv
            if (hasUV) {
                if (idx.texcoord_index >= 0) {
                    int t = idx.texcoord_index;
                    vertices.push_back(attrib.texcoords[2 * t + 0]);
                    vertices.push_back(attrib.texcoords[2 * t + 1]);
                } else {
                    vertices.insert(vertices.end(), {0.f, 0.f});
                }
            }
        }
    }

    if (vertices.empty()) {
        Log::error("Vertex data is empty : " + errID);
        return nullptr;
    }

    return Mesh::create(vertices.data(), vertices.size() * sizeof(float),
                        layout);
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
