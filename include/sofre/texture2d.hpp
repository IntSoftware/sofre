#ifndef SOFRE_TEXTURE2D_HPP
#define SOFRE_TEXTURE2D_HPP

#include <filesystem>
#include <memory>

namespace sofre {

class Texture2D {
public:
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&&) noexcept;
    Texture2D& operator=(Texture2D&&) noexcept;

    void bind(uint32_t unit = 0) const;
    void destroy();

    /**
     * Load texture from file. Returns nullptr on failure.
     */
    static std::shared_ptr<Texture2D> loadFromFile(const std::filesystem::path& path);

    /**
     * Create texture from raw pixel data. Returns nullptr on failure.
     */
    static std::shared_ptr<Texture2D> create(int width, int height, int channels,
                                              const unsigned char* data);


private:
    // Default constructor for internal factory methods
    Texture2D();

    struct Texture2D_GL;
    Texture2D_GL* gl = nullptr;
};

struct TextureBinding {
    std::string uniform;
    std::shared_ptr<Texture2D> texture;
};

} // namespace sofre

#endif // SOFRE_TEXTURE2D_HPP
