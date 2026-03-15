#ifndef SOFRE_TEXTURE2D_HPP
#define SOFRE_TEXTURE2D_HPP

#include <filesystem>
#include <memory>

namespace sofre {

class Texture2D {
public:
    Texture2D(int width, int height, int channels,
              const unsigned char* data);
    Texture2D(const std::filesystem::path& path);

    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&&) noexcept;
    Texture2D& operator=(Texture2D&&) noexcept;

    void bind(uint32_t unit = 0) const;

    template <typename... Args>
    static inline std::shared_ptr<Texture2D> make_texture(Args&&... args) {
        return std::make_shared<Texture2D>(std::forward<Args>(args)...);
    }

private:
    struct Texture2D_GL;
    Texture2D_GL* gl = nullptr;
};

struct TextureBinding {
    std::string uniform;
    std::shared_ptr<Texture2D> texture;
};

} // namespace sofre

#endif // SOFRE_TEXTURE2D_HPP
