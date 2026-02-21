#ifndef SOFRE_TEXTURE2D_HPP
#define SOFRE_TEXTURE2D_HPP

namespace sofre {

class Texture2D {
public:
    Texture2D(int width, int height, int channels,
              const unsigned char* data);

    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&&) noexcept;
    Texture2D& operator=(Texture2D&&) noexcept;

    void bind(uint32_t unit = 0) const;

private:
    struct Texture2D_GL;
    Texture2D_GL* gl = nullptr;
};

} // namespace sofre

#endif // SOFRE_TEXTURE2D_HPP
