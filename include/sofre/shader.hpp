#ifndef SOFRE_SHADER_HPP
#define SOFRE_SHADER_HPP

#include <string>
#include <filesystem>

namespace sofre {

namespace shader {
    static constexpr const char* builtin_viewMatrix = "sofre_ViewMatrix";
    static constexpr const char* builtin_projMatrix = "sofre_ProjMatrix";

    std::string readFile(const std::filesystem::path& path, bool isUTF8withoutBOM = false);

    /** in-place replace non-ASCII to space */
    void utf8_to_ascii_replace(std::string& utf8Str);
    /** write output to parameter 'out' */
    void utf8_to_ascii(const char* data, size_t size, std::string& out);
    void utf16_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);
    void utf32_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);

} // namespace shader

} // namespace sofre

#endif
