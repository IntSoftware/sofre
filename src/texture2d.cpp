#include <sofre/texture2d.hpp>
#include <sofre/log.hpp>

#include <glad/gl.h>
#include <glutil/texture.hpp>

#include <vector>

namespace sofre {

struct Texture2D::Texture2D_GL {
    ~Texture2D_GL() {
        destroy();
    }
    void destroy() {
        if (m_id)
            glDeleteTextures(1, &m_id);
        m_id = 0;
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

Texture2D::Texture2D() : gl(nullptr) {}

std::shared_ptr<Texture2D> Texture2D::loadFromFile(const std::filesystem::path& path) {
    auto tex = glutil::ImageLoader::loadImageToGL(path, true, 0);
    if (!tex.ok || tex.id == 0) {
        Log::err() << "Failed to load texture from file: " << path.string();
        if (!tex.error.empty()) {
            Log::err() << "glutil::ImageLoader error: " << tex.error;
        }
        return nullptr;
    }

    auto result = std::shared_ptr<Texture2D>(new Texture2D());
    result->gl = new Texture2D_GL();
    result->gl->m_id = tex.id;
    tex.id = 0;
    tex.resetInDtor = false;
    return result;
}

std::shared_ptr<Texture2D> Texture2D::create(int width, int height, int channels, const unsigned char* data) {
    if (!data || width <= 0 || height <= 0 || channels <= 0) {
        Log::err() << "Invalid texture parameters: width=" << width
                   << " height=" << height << " channels=" << channels;
        return nullptr;
    }

    auto result = std::shared_ptr<Texture2D>(new Texture2D());
    result->gl = new Texture2D_GL();

    glGenTextures(1, &result->gl->m_id);
    glBindTexture(GL_TEXTURE_2D, result->gl->m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = GL_RGB;
    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 1)
        format = GL_RED;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //TODO_easy : needed?
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return result;
}

Texture2D::~Texture2D() {
    delete gl;
}

void Texture2D::bind(uint32_t unit) const {
    if (!gl || !gl->m_id) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, gl->m_id);
}
void Texture2D::destroy() {
    if (gl)
        gl->destroy();
}
} // namespace sofre
