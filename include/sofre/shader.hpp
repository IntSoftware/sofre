#ifndef SOFRE_SHADER_HPP
#define SOFRE_SHADER_HPP

#include <sofre/enums.hpp>

#include <string>
#include <utility>
#include <filesystem>

namespace sofre {

class Shader {
public:
    Shader(ShaderType type, std::string source) : m_type(type), m_source(std::move(source)) {}
    Shader(ShaderType type, std::filesystem::path sourceFile, bool isUTF8withoutBOM); // TODO : factory?
    ~Shader() {}

    ShaderType type() const { return m_type; }
    const std::string& source() const { return m_source; }

    /** in-place replace non-ASCII to space */
    static void utf8_to_ascii(std::string& utf8Str);
    /** write output to parameter 'out' */
    static void utf8_to_ascii(const char* data, size_t size, std::string& out);
    static void utf16_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);
    static void utf32_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian);

   private:
    ShaderType  m_type{};
    std::string m_source{};
};

} // namespace sofre

#endif
