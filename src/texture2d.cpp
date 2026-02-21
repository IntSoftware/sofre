#include <sofre/texture2d.hpp>
#include "core.hpp"

namespace sofre {

struct Texture2D::Texture2D_GL {
    ~Texture2D_GL() {
        if (m_id)
            glDeleteTextures(1, &m_id);
    }
    GLuint m_id = 0;
};

Texture2D::Texture2D(Texture2D&& other) noexcept : gl(other.gl) {
    other.gl = nullptr;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
    if (this != &other) {
        delete gl;
        gl = other.gl;
        other.gl = nullptr;
    }
    return *this;
}

Texture2D::Texture2D(int width, int height, int channels, const unsigned char* data) {
    gl = new Texture2D_GL();

    glGenTextures(1, &gl->m_id);
    glBindTexture(GL_TEXTURE_2D, gl->m_id);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D() {
    delete gl;
}

void Texture2D::bind(uint32_t unit) const {
    if (!gl || !gl->m_id) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, gl->m_id);
}

} // namespace sofre
