#include <sofre/object.hpp>
#include "core.hpp"

namespace sofre {

struct Object::Object_GL {
    Object_GL() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    }
    ~Object_GL() {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }
    void bind() const {
        glBindVertexArray(m_vao);
    }

   private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};

Object::Object(const float* vertices, size_t size) {
    gl = new Object_GL();
    m_count = static_cast<int>(size / (3 * sizeof(float)));

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);
}

Object::~Object() { delete gl; }

void Object::bind() const { gl->bind(); }

int Object::vertexCount() const { return m_count; }

std::shared_ptr<Object> Object::generate(const float *vertices, size_t size) { 
    return std::shared_ptr<Object>(new Object{vertices, size});
}

}
