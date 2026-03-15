#include <sofre/texture2d.hpp>

#include "core.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

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

namespace {

struct LoadedImage {
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<unsigned char> pixels;
};

LoadedImage loadImageFile(const std::filesystem::path& path) {
    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data =
        stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    if (!data)
        throw std::runtime_error("Failed to load image: " + path.string());

    LoadedImage img;
    img.width = width;
    img.height = height;
    img.channels = channels;
    img.pixels.assign(data, data + (width * height * channels));

    stbi_image_free(data);
    return img;
}

} // anonymous namespace

Texture2D::Texture2D(const std::filesystem::path& path)
{
    auto img = loadImageFile(path);
    *this = Texture2D(img.width, img.height, img.channels, img.pixels.data());
}

Texture2D::Texture2D(int width, int height, int channels, const unsigned char* data) {
    gl = new Texture2D_GL();

    glGenTextures(1, &gl->m_id);
    glBindTexture(GL_TEXTURE_2D, gl->m_id);
    // TODO : how to set these params from user?
    // 1) add enum TextureMode, and call setTextureParam(m_textureMode)
    // to apply some 'presets' about texture filtering, wrapping, mipmap & stuff
    // each TextureMode will set parameters and generate mipmap(if needed) accordingly
    // 2) add Texture options like mipmap, bilinear filtering, make them member function of Textrue2D
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //TODO : needed?

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);

                 
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); //TODO why bind zero?
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
