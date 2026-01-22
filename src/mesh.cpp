#include <sofre/mesh.hpp>
#include "core.hpp"

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

Mesh::Mesh(const float* vertices, size_t size) {
    gl = new Mesh_GL();
    m_count = static_cast<int>(size / (3 * sizeof(float)));

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);
}

Mesh::~Mesh() {
    delete gl;
}

void Mesh::draw() const {
    gl->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_count);
}

}
