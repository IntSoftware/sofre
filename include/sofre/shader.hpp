#ifndef SOFRE_SHADER_HPP
#define SOFRE_SHADER_HPP

#include <sofre/enums.hpp>

#include <string>
#include <utility>
#include <filesystem>

namespace sofre {

class Shader {
public:
    static constexpr const char* builtin_viewMatrix = "sofre_ViewMatrix";
    static constexpr const char* builtin_projMatrix = "sofre_ProjMatrix";

    static inline Shader fromSource(ShaderType type, std::string source) {
        return Shader{type, source};
    }
    static Shader fromFile(ShaderType type,
                            const std::filesystem::path& path,
                            bool isUTF8withoutBOM = false);

    ~Shader() = default;

    ShaderType type() const { return m_type; }
    const std::string& source() const { return m_source; }
    bool valid() const { return !m_source.empty(); }

    /** in-place replace non-ASCII to space */
    static void utf8_to_ascii_replace(std::string& utf8Str);
    /** write output to parameter 'out' */
    static void utf8_to_ascii(const char* data, size_t size, std::string& out);
    static void utf16_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);
    static void utf32_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);

private:
    Shader(ShaderType type, std::string source) : m_type(type), m_source(std::move(source)) {}
    Shader(ShaderType type) : m_type(type) {}

    ShaderType  m_type;
    std::string m_source{};
};

} // namespace sofre

#endif
