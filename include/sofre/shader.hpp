#ifndef SOFRE_SHADER_HPP
#define SOFRE_SHADER_HPP

#include <sofre/enums.hpp>

#include <string>
#include <filesystem>

namespace sofre {

class Shader {
public:
    Shader(ShaderType type, std::string source) : m_type(type), m_source(source) {}
    Shader(ShaderType type, std::filesystem::path sourceFile);
    ~Shader() {}

    ShaderType type() const { return m_type; }
    const std::string& source() const { return m_source; }

    static std::string utf8TOascii(const std::string& utf8Str);

   private:
    ShaderType  m_type{};
    std::string m_source{}; //TODO : should be ASCII
};

} // namespace sofre

#endif
